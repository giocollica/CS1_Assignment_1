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



void fill_monster(monster *m, char *name, char *element, int population)
{
    /* strdup allocates and strcpy's our strings all at once. */

    m->name = strdup(name);
    m->element = strdup(element);
    m->population = population;
}



void remove_crlf(char *s)
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
   s no matter what happens.  Will crash if there isn't a next blank line. 

   YOU ARE ALLOWED TO COPY THIS FUNCTION. */

void get_next_nonblank_line(FILE *ifp, char *s, int max_length)
{
    s[0] = '\0';

    while (s[0] == '\0')
    {
        fgets(s, max_length, ifp);
        remove_crlf(s);
    }
}


/* Function must be changed */
int get_number_of_monsters(FILE *ifp)
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
void read_monster(FILE *ifp, monster *m)
{
    char name[128];
    char element[128];
    char population_string[128];
    int population;

    /* Each monster has a name, a type, a region and a commonality. */
    
    fscanf(ifp, "%s", &name);
    fscanf(ifp, "%s", &element);
    fscanf(ifp, "%s", &population_string);
    population = atoi(population_string);
    fill_monster(m, name, element, population);
}



void print_monster(FILE *ofp, monster *m)
{
    fprintf(ofp, "Monster: %s   Element: %s   Population: %d\n",
            m->name, m->element, m->population);
}


monster *monster_array_constructor(FILE *ifp, FILE *ofp)
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

    for (i = 0; i < numMonsters; i++)
    {
        print_monster(ofp, monsters + i);
    }

    return monsters;
}


int get_number_regions(FILE *ifp)
{
    char s[128];
    int num; 

    get_next_nonblank_line(ifp, s, 127);
    sscanf(s, "%d", &num);

    return num;
}


void fill_region(region *r, char *name, int numMonsters)
{
    r->name = strdup(name);
    r->nmonsters = numMonsters;
}



// **monsters in region is a pointer to the monster array



void read_region(FILE *ifp, region *r)
{
    char name[128];
    char numMonsters_string[128];
    int numMonsters;
    char random[128];
    

    /* Each monster has a name, a type, a region and a commonality. */
    
    get_next_nonblank_line(ifp, name, 127);
    get_next_nonblank_line(ifp, numMonsters_string, 127);
    numMonsters = atoi(numMonsters_string);
 
    // idea for tomorrow
    // have to iterate for each monster based on num monsters
    
    fill_region(r, name, numMonsters);
}



region *region_array_constructor(FILE *ifp, monster *monsters)
{
    int i, j, k;
    int numRegions = get_number_regions(ifp);
    int numMonster;
    region *regions = calloc(numRegions, sizeof(region));
    //three nested loops
        //grab each regions
            //declare double pointer
            malloc(sizeof(region));
            
        //loops through each region to grab monster

        //find corressponding monster struct

    for(i = 0; i < numRegions; i++)
    {
        read_region(ifp, regions + i);
        int numMonstersRegion = get_number_of_monsters(ifp);
        (regions + 1)->monsters = malloc(sizeof(monster*) * numMonstersRegion);

        for(j = 0; j < numMonstersRegion; j++)
        {
            
            for(k = 0; k < temp; k++)
            {
                strcmp((monsters->name) + k, temp);
            }
        }
    }

    return regions;
}




/*
void print_region(FILE *ofp, region *r)
{
    fprintf(ofp, "Region: %s   Number of Monsters: %s\n",
            r->name, r->nmonsters);
}
*/



trainer *new_trainer_array(int numTrainers)
{
    trainer *t = calloc(numTrainers, sizeof(trainer));

    return t;
}


int get_number_trainers(FILE *ifp)
{
    char s[128];
    int num; 

    get_next_nonblank_line(ifp, s, 127);
    sscanf(s, "%d", &num);

    return num;
}




/*


void fill_trainer(region *r, char *name, int numMonsters, monster *monsters)
{
    r->name = strdup(name);
    r->nmonsters = numMonsters;
    r->monsters = strdup(monsters->name);
}


void read_trainer(FILE *ifp, region *r)
{
    char name[128];
    char numMonsters_string[128];
    int numMonsters;
    monster *monsters;

    // Each monster has a name, a type, a region and a commonality.
    
    get_next_nonblank_line(ifp, name, 127);
    get_next_nonblank_line(ifp, numMonsters_string, 127);
    get_next_nonblank_line(ifp, monsters->name, 127);
    numMonsters = atoi(numMonsters_string);
    // idea for tomorrow
    // have to iterate for each monster based on num monsters
    
    fill_region(r, name, numMonsters, monsters->name);
}
*/


/*
    Read in name of trainers
        read in itinerary function
*/





int main()
{
    atexit(report_mem_leak);
    FILE *ifp;
    FILE *ofp;

    int numRegions, numTrainers;
    int i;
    monster *monsters;
    region *regions;
    trainer *trainers;

    ifp = fopen("input.txt", "r");
    ofp = fopen("output.txt", "w");

    monsters = monster_array_constructor(ifp, ofp);
    regions = region_array_constructor(ifp, monsters);

/*
    numRegions = get_number_regions(ifp);

    fprintf(ofp, "Hello, world!  I should have %d regions.\n", numRegions);

    regions = new_region_array(numRegions);

    for (i = 0; i < numRegions; i++)
    {
        read_region(ifp, regions + i); 
    }
*/
/*
    for (i = 0; i < numRegions; i++)
    {
        print_region(ofp, regions + i); 
    }
*/

    numTrainers = get_number_trainers(ifp);

    fprintf(ofp, "Hello, world!  I should have %d regions.\n", numTrainers);

    trainers = new_trainer_array(numTrainers);

    
    return 0;

}

