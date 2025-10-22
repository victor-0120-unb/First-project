#include<stdio.h>
#include<stdlib.h>

int max[100][100];
int alloc[100][100];
int need[100][100];  // Made global so request() can access it
int avail[100];
int n, r;

void input();
void show();
void cal();
void request();

int main() {
    printf(" Banker's Algorithm \n\n");
    input();
    show();
    cal();
    request();
    return 0;
}

void input() {
    int i, j;
    
    printf("Enter the number of Processes: ");
    scanf("%d", &n);
    
    printf("Enter the number of Resource types: ");
    scanf("%d", &r);
    
    printf("\n--- Enter the Max Matrix ---\n");
    printf("(Enter %d values for each process,one value at a time for %d processes total)\n", r, n);
    for (i = 0; i < n; i++) {
        printf("Process P%d (enter %d values): ", i, r);
        for (j = 0; j < r; j++) {
            scanf("%d", &max[i][j]);
        }
    }
    
    printf("\n--- Enter the Allocation Matrix ---\n");
    printf("(Enter %d values for each process,one value at a time for %d processes total)\n", r, n);
    for (i = 0; i < n; i++) {
        printf("Process P%d (enter %d values): ", i, r);
        for (j = 0; j < r; j++) {
            scanf("%d", &alloc[i][j]);
        }
    }
    
    printf("\n--- Enter the Available Resources ---\n");
    printf("(Enter %d values,each at a time. one for each resource type): ", r);
    for (j = 0; j < r; j++) {
        scanf("%d", &avail[j]);
    }
}

void show() {
    int i, j;
    
    printf("\n\n========== Current System State ==========\n");
    printf("Process\t Allocation\t Max\t\t Need\t\t Available\n");
    
    // Calculate Need matrix first
    for (i = 0; i < n; i++) {
        for (j = 0; j < r; j++) {
            need[i][j] = max[i][j] - alloc[i][j];
        }
    }
    
    for (i = 0; i < n; i++) {
        printf("P%d\t ", i);
        
        // Allocation
        for (j = 0; j < r; j++) {
            printf("%d ", alloc[i][j]);
        }
        printf("\t\t");
        
        // Max
        for (j = 0; j < r; j++) {
            printf("%d ", max[i][j]);
        }
        printf("\t\t");
        
        // Need
        for (j = 0; j < r; j++) {
            printf("%d ", need[i][j]);
        }
        printf("\t\t");
        
        // Available (only on first row)
        if (i == 0) {
            for (j = 0; j < r; j++)
                printf("%d ", avail[j]);
        }
        printf("\n");
    }
}

void cal() {
    int finish[100], flag = 1, k, c1 = 0;
    int safe[100];
    int i, j;
    int work[100];
    
    // Initialize work as available
    for (j = 0; j < r; j++) {
        work[j] = avail[j];
    }
    
    for (i = 0; i < n; i++) {
        finish[i] = 0;
    }
    
    // Calculate need matrix
    for (i = 0; i < n; i++) {
        for (j = 0; j < r; j++) {
            need[i][j] = max[i][j] - alloc[i][j];
        }
    }
    
    printf("\n\n========== Safety Algorithm ==========\n");
    printf("Safe Sequence: ");
    
    while (flag) {
        flag = 0;
        for (i = 0; i < n; i++) {
            if (finish[i] == 0) {
                int c = 0;
                for (j = 0; j < r; j++) {
                    if (need[i][j] <= work[j]) {
                        c++;
                    }
                }
                
                if (c == r) {
                    printf("P%d ", i);
                    for (k = 0; k < r; k++) {
                        work[k] += alloc[i][k];  // FIXED: was alloc[i][j]
                    }
                    finish[i] = 1;
                    flag = 1;
                    safe[c1++] = i;
                }
            }
        }
    }
    
    // Check if all processes finished
    int all_finished = 1;
    for (i = 0; i < n; i++) {
        if (finish[i] == 0) {
            all_finished = 0;
            printf("\nUnfinished: P%d ", i);
        }
    }
    
    if (all_finished) {
        printf("\n\nResult: The system is in SAFE state\n");
    } else {
        printf("\n\nResult: The system is in UNSAFE state (Deadlock possible)\n");
    }
}

void request() {
    int c, pid, request[100], i;
    int work[100];
    
    printf("\n\n========== Resource Request ==========\n");
    printf("Do you want to make a resource request? (1=Yes | 0=No): ");
    scanf("%d", &c);
    
    if (c == 1) {
        printf("\nEnter process number (0 to %d): ", n-1);
        scanf("%d", &pid);
        
        if (pid < 0 || pid >= n) {
            printf("\nError: Invalid process number!\n");
            return;
        }
        
        printf("\nEnter request for each resource type (%d values):\n", r);
        for (i = 0; i < r; i++) {
            printf("  Resource %d: ", i);
            scanf("%d", &request[i]);
        }
        
        // Check if request <= need
        for (i = 0; i < r; i++) {
            if (request[i] > need[pid][i]) {
                printf("\nError: Process has exceeded its maximum claim!\n");
                printf("Requested: %d, But Need is: %d for resource %d\n", 
                       request[i], need[pid][i], i);
                return;
            }
        }
        
        // Check if request <= available
        for (i = 0; i < r; i++) {
            if (request[i] > avail[i]) {
                printf("\nProcess must wait - resources not available!\n");
                printf("Requested: %d, But Available is: %d for resource %d\n", 
                       request[i], avail[i], i);
                return;
            }
        }
        
        // Try allocating resources
        printf("\nTrying to allocate resources...\n");
        for (i = 0; i < r; i++) {
            avail[i] -= request[i];
            alloc[pid][i] += request[i];
            need[pid][i] -= request[i];
        }
        
        printf("\nChecking if new state is safe...\n");
        cal();
    }
}

