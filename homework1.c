#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "leak_detector_c.h"

typedef struct monster 
{ 
    int id; 
    char *name; 
    char *element; 
    int population; 
} monster; 

typedef struct region 
{ 
    char *name; 
    int nmonsters; 
    int total_population; 
    monster **monsters; 
} region; 

typedef struct itinerary 
{ 
    int nregions; 
    region **regions; 
    int captures; 
} itinerary; 

typedef struct trainer 
{ 
    char *name; 
    itinerary *visits; 
} trainer;


//function to fill in elements of monster array
static void fill_monster(monster *m, char *name, char *element, int population)
{
    m->name = strdup(name);
    m->element = strdup(element);
    m->population = population;
}


static void remove_crlf(char *s)
{
    char *t = s + strlen(s);

    // t begins at the null sentinel at the end of s.

    t--;

    /* t is now at the last character of s - unless s didn't contain any characters, in which
       case, t is now *BEFORE* s.  We have to keep checking for that. */

    /* We repeat until EITHER t slides to the left of s, OR we find a character that is not a
       line feed (\n) or a carriage return (\r). */

    while ((t >= s) && (*t == '\n' || *t == '\r'))
    {
        *t = '\0'; // Clobber the character t is pointing at.
        t--;      // Decrement t.
    }
}

/* Get the next line from an input file that isn't blank, and leave it in s.  Will clobber
   s no matter what happens.  Will crash if there isn't a next blank line. */

static void get_next_nonblank_line(FILE *ifp, char *s, int max_length)
{
    s[0] = '\0';

    while (s[0] == '\0')
    {
        fgets(s, max_length, ifp);
        remove_crlf(s);
    }
}

//function to get the number of monsters
static int get_number_of_monsters(FILE *ifp)
{
    char s[128];
    int num;

    // Get the first line of the file.
    fgets(s, 127, ifp);

    //pull out number from the file
    sscanf(s, "%d", &num);

    return num;
}

//function to read in monsters from the file
//calls fill_monster function
static void read_monster(FILE *ifp, monster *m)
{
    char name[128];
    char element[128];
    char population_string[128];
    int population;
    
    fscanf(ifp, "%s", name);
    fscanf(ifp, "%s", element);
    fscanf(ifp, "%s", population_string);
    population = atoi(population_string);
    fill_monster(m, name, element, population);
}

//monster array constructor to fill in all elements of the monster array
static monster *monster_array_constructor(FILE *ifp)
{
    int i = 0;
    //call number of monsters
    int numMonsters = get_number_of_monsters(ifp);
    //malloc memory to pointer handed for nmonsters
    monster *monsters = calloc(numMonsters, sizeof(monster));
    //loop through calling fill monster
    for (i = 0; i < numMonsters; i++)
    {
        read_monster(ifp, monsters + i);
    }

    return monsters;
}

//gets the number of regions from the file
static int get_number_regions(FILE *ifp)
{
    char s[128];
    int num; 

    get_next_nonblank_line(ifp, s, 127);
    sscanf(s, "%d", &num);

    return num;
}

//function to fill in elements of region array
static void fill_region(region *r, char *name, int numMonsters)
{
    r->name = strdup(name);
    r->nmonsters = numMonsters;
}

//function to read in elements of regions from the file
//calls the fill_region function
static void read_region(FILE *ifp, region *r)
{
    char name[128];
    char numMonsters_string[128];
    int numMonsters;
    
    get_next_nonblank_line(ifp, name, 127);
    get_next_nonblank_line(ifp, numMonsters_string, 127);
    numMonsters = atoi(numMonsters_string);
    
    fill_region(r, name, numMonsters);
}

//function to get the total population for the region
static region *get_total_population(region *regions, int numRegions)
{
    int i, j;
    for(i = 0; i < numRegions; i++)
    {
        for(j = 0; j < (regions + i)->nmonsters; j++)
        {
            (regions + i)->total_population += (regions + i)->monsters[j]->population;
        }
    }
    return regions;
}

//region array constructor to fill in elements of regions array
static region *region_array_constructor(FILE *ifp, monster *m)
{
    int i, j, k;
    int numRegions = get_number_regions(ifp);
    region *regions = calloc(numRegions, sizeof(region));

    /*
        goes through the number of regions, the number of monsters
        in each region to assign the monster double pointer in the
        regions struct to adress of the corresponding monster.
    */
    for(i = 0; i < numRegions; i++)
    {
        read_region(ifp, regions + i);
        (regions + i)->monsters = malloc(sizeof(monster*) * (regions + i)->nmonsters);
        for(j = 0; j < (regions + i)->nmonsters; j++)
        {
            char buf[128];
            get_next_nonblank_line(ifp, buf, 127);     
            for(k = 0; /*go untill break*/; k++)
            {
                int tmp = 0;
                tmp = strcmp( ((m + k)->name), buf );
                if(tmp == 0){
                    (regions + i)->monsters[j] = (m + k);
                    break;
                }
            }
        }
        
    }

    get_total_population(regions, numRegions);

    return regions;
}

//function to read number of trainers from the file
static int get_number_trainers(FILE *ifp)
{
    char s[128];
    int num; 

    get_next_nonblank_line(ifp, s, 127);
    sscanf(s, "%d", &num);

    return num;
}

//function to read number of captures from the file
static int get_number_captures(FILE *ifp){
    char s[128];
    int num; 

    get_next_nonblank_line(ifp, s, 127);
    sscanf(s, "%d", &num);

    return num;
}

//function to fill in elements of trainer array
static void fill_trainer(trainer *t, char *name)
{
    t->name = strdup(name);   
}

//function to read in the name of the trainer
//calls fill_trainer function
static void read_trainer(FILE *ifp, trainer *t)
{
    char name[128];

    get_next_nonblank_line(ifp, name, 127);  

    fill_trainer(t, name);
}

//function to fill in elements of itinerary array
static void fill_itinerary(itinerary *itin, int numCap, int numRegion)
{
    itin->captures = numCap;
    itin->nregions = numRegion;
}

//itinerary array constructor to fill in elements of the itinerary array
static itinerary *itinerary_array_constructor(FILE *ifp, region *r, int numTrainers)
{
    int i, j;
    int numCaptures = get_number_captures(ifp);
    int numRegions = get_number_regions(ifp);
    itinerary *trainerItinerary = calloc(numTrainers, sizeof(itinerary));
    fill_itinerary(trainerItinerary, numCaptures, numRegions);
    
    /*
        goes through the number of regions to assign the regions
        double pointer in the itinerary struct to the corresponding
        region
    */
    for(i = 0; i < numRegions; i++)
    {
        (trainerItinerary + i)->regions = malloc(sizeof(region*) * numRegions);
        char buf[128];
        get_next_nonblank_line(ifp, buf, 127);
        for(j = 0; /*go until break*/; j++)
        {
            int tmp = 0;
            tmp = strcmp( ((r + j)->name), buf );
            if(tmp == 0){
                (trainerItinerary)->regions[i] = (r + j);
                break;
            }
        }
        
    }
    return trainerItinerary;
}

//trainer array constructor to fill in elements of trainer array
static trainer *trainer_array_constructor(FILE *ifp, region *r, int *numTrain)
{
    int i = 0;
    int numTrainers = 0;
    numTrainers = get_number_trainers(ifp);
    *numTrain = numTrainers;
    trainer *trainers = calloc(numTrainers, sizeof(trainer));
    
    //run through number of trainers to fill in elements
    //for each element in the array
    for(i = 0; i < numTrainers; i++)
    {
        read_trainer(ifp, (trainers + i));
        (trainers + i)->visits = malloc(sizeof(itinerary*) * numTrainers);
        (trainers + i)->visits = itinerary_array_constructor(ifp, r, numTrainers);
    }

    return trainers;
}

//function to print out the desired output set in the assignment
static void print_output(FILE *ofp, trainer *trainers, int numTrainers)
{
    int i, j, k;
    for(i = 0; i < numTrainers; i++)
    {
        fprintf(ofp, "\n%s\n", (trainers + i)->name);
        for(j = 0; j < (trainers + i)->visits->nregions; j++)
        {
            fprintf(ofp, "%s\n", (trainers + i)->visits->regions[j]->name);
            for(k = 0; k < (trainers + i)->visits->regions[j]->nmonsters; k++)
            {
                double monsterPop = ((trainers + i)->visits->regions[j]->monsters[k]->population);
                double regionPop = ((trainers + i)->visits->regions[j]->total_population);
                double intendedCaptures = ((trainers + i)->visits->captures);
                double temp = (monsterPop / regionPop) * intendedCaptures;
                int numCaptured = round(temp);
                if(numCaptured > 0)
                {
                    fprintf(ofp, "%d %s\n", numCaptured, (trainers + i)->visits->regions[j]->monsters[k]->name);
                }
            }
        }
    }
}

//main function
int main(void)
{
    atexit(report_mem_leak);
    
    FILE *ifp;
    FILE *ofp;

    monster *monsters;
    region *regions;
    trainer *trainers;

    int numTrainers;

    ifp = fopen("input.txt", "r");
    ofp = fopen("output.txt", "w");

    monsters = monster_array_constructor(ifp);
    regions = region_array_constructor(ifp, monsters);
    trainers = trainer_array_constructor(ifp, regions, &numTrainers);
    
    print_output(ofp, trainers, numTrainers);

    return 0;

}

