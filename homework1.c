#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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



static void fill_monster(monster *m, char *name, char *element, int population)
{
    /* strdup allocates and strcpy's our strings all at once. */

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


/* Function must be changed */
static int get_number_of_monsters(FILE *ifp)
{
    char s[128];
    int num;

    // Get the first line of the file.
    fgets(s, 127, ifp);

    // The line should be formatted as "<number> monsters".  Pull off the number.
    sscanf(s, "%d", &num);

    return num;
}


//rename to read and fill monster
static void read_monster(FILE *ifp, monster *m)
{
    char name[128];
    char element[128];
    char population_string[128];
    int population;

    /* Each monster has a name, a type, a region and a commonality. */
    
    fscanf(ifp, "%s", name);
    fscanf(ifp, "%s", element);
    fscanf(ifp, "%s", population_string);
    population = atoi(population_string);
    fill_monster(m, name, element, population);
}



static void print_monster(FILE *ofp, monster *m)
{
    fprintf(ofp, "Monster: %s   Element: %s   Population: %d\n",
            m->name, m->element, m->population);
}


static monster *monster_array_constructor(FILE *ifp, FILE *ofp)
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


static int get_number_regions(FILE *ifp)
{
    char s[128];
    int num; 

    get_next_nonblank_line(ifp, s, 127);
    sscanf(s, "%d", &num);

    return num;
}


static void fill_region(region *r, char *name, int numMonsters)
{
    r->name = strdup(name);
    r->nmonsters = numMonsters;
}



// **monsters in region is a pointer to the monster array



static void read_region(FILE *ifp, region *r)
{
    char name[128];
    char numMonsters_string[128];
    int numMonsters;
    //char random[128];
    

    /* Each monster has a name, a type, a region and a commonality. */
    
    get_next_nonblank_line(ifp, name, 127);
    get_next_nonblank_line(ifp, numMonsters_string, 127);
    numMonsters = atoi(numMonsters_string);
 
    // idea for tomorrow
    // have to iterate for each monster based on num monsters
    
    fill_region(r, name, numMonsters);
}



static region *region_array_constructor(FILE *ifp, monster *m)
{
    int i, j, k;
    int numRegions = get_number_regions(ifp);
    //int numMonster;
    region *regions = calloc(numRegions, sizeof(region));

    for(i = 0; i < numRegions; i++)
    {
        read_region(ifp, regions + i);
        //int numMonstersRegion = get_number_of_monsters(ifp);
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

    return regions;
}


static int get_number_trainers(FILE *ifp)
{
    char s[128];
    int num; 

    get_next_nonblank_line(ifp, s, 127);
    sscanf(s, "%d", &num);

    return num;
}


static int get_number_captures(FILE *ifp){
    char s[128];
    int num; 

    get_next_nonblank_line(ifp, s, 127);
    sscanf(s, "%d", &num);

    return num;
}



static void fill_trainer(trainer *t, char *name)
{
    t->name = strdup(name);   
}



static void read_trainer(FILE *ifp, trainer *t)
{
    char name[128];

    get_next_nonblank_line(ifp, name, 127);  

    fill_trainer(t, name);
}

static void fill_itinerary(itinerary *itin, int numCap, int numRegion)
{
    itin->captures = numCap;
    itin->nregions = numRegion;
}



static itinerary *itinerary_array_constructor(FILE *ifp, region *r, int numTrainers)
{
    int i, j;
    int numCaptures = get_number_captures(ifp);
    int numRegions = get_number_regions(ifp);
    itinerary *trainerItinerary = calloc(numTrainers, sizeof(itinerary));
    fill_itinerary(trainerItinerary, numCaptures, numRegions);
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



static trainer *trainer_array_constructor(FILE *ifp, region *r, int *numTrain)
{
    int i = 0;
    int numTrainers = 0;
    numTrainers = get_number_trainers(ifp);
    *numTrain = numTrainers;
    trainer *trainers = calloc(numTrainers, sizeof(trainer));
    
    for(i = 0; i < numTrainers; i++)
    {
        read_trainer(ifp, (trainers + i));
        (trainers + i)->visits = malloc(sizeof(itinerary*) * numTrainers);
        (trainers + i)->visits = itinerary_array_constructor(ifp, r, numTrainers);
    }

    return trainers;
}


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
                fprintf(ofp, "# %s\n", (trainers + i)->visits->regions[j]->monsters[k]->name);
            }
        }
    }
}




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

    monsters = monster_array_constructor(ifp, ofp);
    regions = region_array_constructor(ifp, monsters);
    trainers = trainer_array_constructor(ifp, regions, &numTrainers);

    //function to calculate captures
    
    print_output(ofp, trainers, numTrainers);

    return 0;

}

