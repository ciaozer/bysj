#include "../include/main.h"
#include "../include/generate.h"
#include "../include/read.h"
#include "../include/algorithm.h"

extern char *instance;
extern double DENSITY;          // the density of conflict graph
extern double RATE;             // the posibility add into solution
extern int AVG_COVER_TIMES;     // the average of element be covered times
extern int ITEMNUM;
extern int ELEMENTNUM;

void parse_args(int argc, char *argv[]){
    if ( argc < 2 ) {
        cerr << "Usage: " << argv[0] << " [function] [parameters...]" << endl;
        cout << "function:" << endl;
        cout << "-g: geneate data" << endl;
        cout << "example: ./main -g itemnum elementnum density avg_cover_times path" << endl;
        cout << "-s: change to sat" << endl;
        cout << "example: ./main -s path_of_scpc path_of_sat rate" << endl;
        cout << "-r: run" << endl;
        cout << "example: ./main -r path" << endl;
        fflush(stdout);
        return ;
    }
    
    if ( string(argv[1]).compare(string("-g")) == 0 )
    {
        ITEMNUM = atoi(argv[2]);
        ELEMENTNUM = atoi(argv[3]);
        DENSITY = atof(argv[4]);
        AVG_COVER_TIMES = atoi(argv[5]);
        cout << "generating data..." << endl;
        printf("ITEMNUM = %d\nELEMENTNUM = %d\nDENSITY = %lf\nAVG_COVER_TIMES = %d\npath = %s\n", ITEMNUM, ELEMENTNUM, DENSITY, AVG_COVER_TIMES, argv[6]);
        generate_random_once(argv[6]);
    }
    
    else if ( string(argv[1]).compare(string("-r")) == 0 ){
        run(argv[2]);
    }

    else if ( string(argv[1]).compare(string("-s")) == 0 )
    {
        cout << "changing to sat" << endl;
        change_to_sat(argv[2], argv[3], atof(argv[4]));
    }

    else
    {
        cerr << "Usage: " << argv[0] << " [function] [parameters...]" << endl;
        cout << "function:" << endl;
        cout << "-g: geneate data" << endl;
        cout << "example: ./main -g itemnum elementnum density avg_cover_times path" << endl;
        cout << "-s: change to sat" << endl;
        cout << "example: ./main -s path_of_scpc path_of_sat rate" << endl;
        cout << "-r: run" << endl;
        cout << "example: ./main -r path" << endl;
        fflush(stdout);
        return ;
    }
}