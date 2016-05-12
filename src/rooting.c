#include <xmtc.h>

//#define PRINT
//#define PRINT_FINAL
//#define NO_LIST_RANK


//#define TEST
//#define N 6
//#define M 10


int next_write[M];
int next_read[M];
int rank_read[M] = {0};
int rank_write[M] = {0};


int main(void)
{
    int i,j;

#ifdef TEST
    root = 3;
    
    V[0] = 0;
    V[1] = 1;
    V[2] = 4;
    V[3] = 5;
    V[4] = 8;
    V[5] = 9;
    
    deg[0] = 1;
    deg[1] = 3;
    deg[2] = 1;
    deg[3] = 3;
    deg[4] = 1;
    deg[5] = 1;
    
    E[0][0] = 0; E[0][1] = 1;
    E[1][0] = 1; E[1][1] = 0;
    E[2][0] = 1; E[2][1] = 2;
    E[3][0] = 1; E[3][1] = 3;
    E[4][0] = 2; E[4][1] = 1;
    E[5][0] = 3; E[5][1] = 1;
    E[6][0] = 3; E[6][1] = 4;
    E[7][0] = 3; E[7][1] = 5;
    E[8][0] = 4; E[8][1] = 3;
    E[9][0] = 5; E[9][1] = 3;
    
    ptr[0] = 1;
    ptr[1] = 0;
    ptr[2] = 4;
    ptr[3] = 5;
    ptr[4] = 2;
    ptr[5] = 3;
    ptr[6] = 8;
    ptr[7] = 9;
    ptr[8] = 6;
    ptr[9] = 7;
#endif

#ifdef PRINT
    printf ("N is : %d\n", N);
    printf ("M is : %d\n", M);
    printf("root is : %d\n\n\n", root);

    printf("Vertex list is as follows : \n");
    for (i=0; i<N; i++) {
        printf("%d  ", V[i]);
    }
    printf("\n\n\n");
    
    printf("Degree of Vertex is as follows : \n");
    for (i=0; i<N; i++) {
        printf("%d  ", deg[i]);
    }
    printf("\n\n\n");
    
    
    printf("Edge list is as follows : \n");
    for (i=0; i<M; i++) {
        printf(" ( %d -> %d ) ", E[i][0], E[i][1]);
    }
    printf("\n\n\n");

    printf("ptr list is as follows : \n");
    for (i=0; i<M; i++) {
        printf("%d  ", ptr[i]);
    }
    printf("\n\n\n");
    
    int offset = 0;
    for (i=0; i<N; i++) {
        printf("Vertex : %d \n", i);
        printf(" Edges Are : ");
        for (j=0; j<deg[i]; j++) {
            printf(" ( %d -> %d ) ", E[V[i]+j][0], E[V[i]+j][1]);
        }
        printf("\n");

        printf("A-Edges Are :");
        for (j=0; j<deg[i]; j++) {
            printf(" ( %d -> %d ) ", E[ptr[j+offset]][0], E[ptr[j+offset]][1]);
        }
        offset += deg[i];
        printf("\n\n");
    }
#endif
    
    spawn(0,M-1){
        int relative_idx, start_ptr_node;
        start_ptr_node = V[E[$][1]];
        relative_idx = ptr[$] - start_ptr_node;
        next_read[$] = start_ptr_node + ((relative_idx + 1 )% deg[E[$][1]]);
    }
    next_read[ptr[V[root]]] = NIL;
    
    
#ifdef PRINT
    for (i=0; i<M; i++) {
        printf ("next_read : %d\n", next_read[i]);
    }
    printf("\n\n\n");
#endif
   
#ifdef PRINT
    printf("Rankings are as follows : \n");
    for (i=0; i<M; i++) {
        printf("( %d -> %d ) : %d\n", E[i][0], E[i][1], rank_read[i]);
    }
    printf("\n\n\n");
    printf("Starting the Ranking Algorithm...\n\n\n\n");
#endif
    
    //Apply List Ranking Algorithm
    int log_M = 0;
    int cnt = M;
    while (cnt > 1) {
        log_M++;
        cnt = cnt/2;
    }
    log_M++;

#ifdef PRINT
    printf("log_M : %d\n\n", log_M);
#endif

    spawn(0,M-1){
        if (next_read[$] == NIL) {rank_read[$] = 0;}
        else rank_read[$] = 1;
    }
    int k = 0;
    for (k = 0; k<log_M; k++) {
        spawn(0,M-1){
            if(k%2==0){
                int nxt = next_read[$];
                if(nxt == NIL){
                    next_write[$] = NIL;
                    rank_write[$] = rank_read[$];
                }
                else{
                    rank_write[$] = rank_read[$] + rank_read[nxt];
                    next_write[$] = next_read[nxt];
                }
            }
            else{
                int nxt = next_write[$];
                if(nxt == NIL){
                    next_read[$] = NIL;
                    rank_read[$] = rank_write[$];
                }
                else{
                    rank_read[$] = rank_write[$] + rank_write[nxt];
                    next_read[$] = next_write[nxt];
                }
            }
            
        }
        
        /*
        spawn(0,M-1){
                rank_read[$] = rank_write[$];
                next_read[$] = next_write[$];
        }
         */
    }

    
#ifdef NO_LIST_RANK
    /*
    rank[0] = 3;
    rank[1] = 4;
    rank[2] = 2;
    rank[3] = 0;
    rank[4] = 1;
    rank[5] = 5;
    rank[6] = 9;
    rank[7] = 7;
    rank[8] = 8;
    rank[9] = 6;
     */
#endif

    
#ifdef PRINT
    for (i=0; i<M; i++) {
        printf ("next_read : %d\n", next_read[i]);
    }
    printf("\n\n\n");
    
    printf("Rankings are as follows : \n");
    for (i=0; i<M; i++) {
        printf("( %d -> %d ) : %d\n", E[i][0], E[i][1], rank_read[i]);
    }
    printf("\n\n\n");
#endif
    
    spawn(0,M-1){
        int idx;
        int n_ptr, ant_ptr;
        n_ptr = $;
        ant_ptr = ptr[n_ptr];
        if (rank_read[n_ptr] < rank_read[ant_ptr]) used[n_ptr] = 1;
        else used[ant_ptr] = 1;
    }

    
#ifdef PRINT_FINAL
    printf("Used Edges are as follows : \n");
    for (i=0; i<M; i++) {
        if(used[i]) printf("( %d -> %d )\n", E[i][0], E[i][1]);
        else if (used[i] == NIL) { printf("** error ** \n");}
    }
    printf("\n\n\n");
#endif
    
    return 0;
}
