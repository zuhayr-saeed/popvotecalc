#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

#include "MinPopVote.h"

bool setSettings(int argc, char** argv, int* year, bool* fastMode, bool* quietMode) {
    // Set default values
  *year = 0;
  *fastMode = false;
  *quietMode = false;

  for (int i = 1; i < argc; i++) {
      if (strcmp(argv[i], "-f") == 0) {
          *fastMode = true;
      }
      else if (strcmp(argv[i], "-q") == 0) {
          *quietMode = true;
      }
      else if (strcmp(argv[i], "-y") == 0) {
          // Check if there's a year argument following -y
          if (i + 1 < argc) {
              *year = atoi(argv[i + 1]);
              if (!(*year >= 1828 && *year <= 2020 && *year % 4 == 0)) {
                  *year = 0;
              }
              i++;
          }
      }
      else {
          // Invalid command-line argument
          return false;
      }
  }

  return true; 
}

void inFilename(char* filename, int year) {
    // Generate the input file name in the format data/[year].csv
    sprintf(filename, "data/%d.csv", year);
    return;
}

void outFilename(char* filename, int year) {
    // Generate the output file name in the format toWin/[year]_win.csv
    sprintf(filename, "toWin/%d_win.csv", year);
    return;
}

bool parseLine(char* line, State* myState) {
    // Remove the newline character, if present
    int length = strlen(line);
    if (length > 0 && line[length - 1] == '\n') {
        line[length - 1] = '\0';
    }

    // Split the line using commas
    char* token = strtok(line, ",");
    if (token == NULL) {
        return false;
    }
    strcpy(myState->name, token);

    token = strtok(NULL, ",");
    if (token == NULL) {
        return false;
    }
    strcpy(myState->postalCode, token);

    token = strtok(NULL, ",");
    if (token == NULL) {
        return false;
    }
    myState->electoralVotes = atoi(token);

    token = strtok(NULL, ",");
    if (token == NULL) {
        return false;
    }
    myState->popularVotes = atoi(token);

    return true;  // Successfully parsed the line
}

bool readElectionData(char* filename, State* allStates, int* nStates) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        // Return false if the file cannot be found
        return false;
    }

    *nStates = 0;  // Initialize the number of states
    State currentState;

    char line[100];  // Adjust the size according to your needs

    while (fgets(line, sizeof(line), file) != NULL) {
        if(*nStates >= 51) {
            // Prevent going beyond the maximum number of states
            break;
        }
        // Parse the line and increment the number of states if successful
        if (parseLine(line, &currentState)) {
            allStates[*nStates] = currentState;
            (*nStates)++;
        } 
    }

    // Print the parsed data
    // printf("Electoral Data for %s (%s):\n", currentState.name, currentState.postalCode);
    // printf("  Electoral Votes = %d\n", currentState.electoralVotes);
    // printf("  Popular Votes = %d\n\n", currentState.popularVotes);

    fclose(file);

    return true;
}

int totalEVs(State* states, int szStates) {
    int totalElectoralVotes = 0;
    for (int i = 0; i < szStates; i++) {
        totalElectoralVotes += states[i].electoralVotes;
    }
    return totalElectoralVotes;
    //return 0; // modify or replace this line
}

int totalPVs(State* states, int szStates) {

    int totalPopularVotes = 0;
    for (int i = 0; i < szStates; i++) {
        totalPopularVotes += states[i].popularVotes;
    }
    return totalPopularVotes;
}

MinInfo minPopVoteAtLeast(State* states, int szStates, int start, int EVs) {
    MinInfo res;
    res.szSomeStates = 0;
    res.subsetPVs = 0;

    MinInfo include;          
    include.szSomeStates = 0;

    MinInfo exclude; // modify or replace this line
    exclude.szSomeStates = 0;

    if (start == szStates) {
        res.sufficientEVs = (EVs <= 0);
        return res;
    }

    if (EVs <= 0) {
        res.sufficientEVs = true;
        return res;
    }

    include = minPopVoteAtLeast(states, szStates, start + 1, EVs - states[start].electoralVotes);
    include.someStates[include.szSomeStates] = states[start];
    include.szSomeStates++;
    include.subsetPVs = (totalPVs(include.someStates, include.szSomeStates) / 2 + 1);

    exclude = minPopVoteAtLeast(states, szStates, start + 1, EVs);
    exclude.subsetPVs = (totalPVs(exclude.someStates, exclude.szSomeStates) / 2 + 1);

    if (include.sufficientEVs && exclude.sufficientEVs) {
        if (include.subsetPVs < exclude.subsetPVs) {
            res = include;
        } else {
            res = exclude;
        }
    } else if (include.sufficientEVs) {
        res = include;

    } else {
        res = exclude;
    }
    res.subsetPVs = 0;

    for(int i = 0; i < res.szSomeStates; i++) {
        res.subsetPVs += (res.someStates[i]).popularVotes / 2 + 1;
    }

    return res;
}


MinInfo minPopVoteToWin(State* states, int szStates) {
    int totEVs = totalEVs(states,szStates);
    int reqEVs = totEVs/2 + 1; // required EVs to win election
    return minPopVoteAtLeast(states, szStates, 0, reqEVs);
}

MinInfo minPopVoteAtLeastFast(State* states, int szStates, int start, int EVs, MinInfo** memo) {
    MinInfo res;

    // Check for base cases
    if (start == szStates) {
        MinInfo res;
        res.szSomeStates = 0;
        res.subsetPVs = 0;
        res.sufficientEVs = (EVs <= 0);
        return res;
    }

    if (EVs <= 0) {
        MinInfo res;
        res.szSomeStates = 0;
        res.subsetPVs = 0;
        res.sufficientEVs = true;
        return res;
    }

    // Check if the result for the current [start][EVs] has already been calculated
    if (memo[start][EVs].subsetPVs != -1) {
        return memo[start][EVs];
    }

    MinInfo include = minPopVoteAtLeastFast(states, szStates, start + 1, EVs - states[start].electoralVotes, memo);
    include.someStates[include.szSomeStates] = states[start];
    include.szSomeStates++;
    include.subsetPVs = (totalPVs(include.someStates, include.szSomeStates) / 2 + 1);

    MinInfo exclude = minPopVoteAtLeastFast(states, szStates, start + 1, EVs, memo);
    exclude.subsetPVs = (totalPVs(exclude.someStates, exclude.szSomeStates) / 2 + 1);

    if (include.sufficientEVs && exclude.sufficientEVs) {
        if (include.subsetPVs < exclude.subsetPVs) {
            res = include;
        } else {
            res = exclude;
        }
    } else if (include.sufficientEVs) {
        res = include;
    } else {
        res = exclude;
    }

    res.subsetPVs = 0;
    for (int i = 0; i < res.szSomeStates; i++) {
        res.subsetPVs += (res.someStates[i]).popularVotes / 2 + 1;
    }

    // Save the result in the memoization table for future use
    memo[start][EVs] = res;

    return res;
}

MinInfo minPopVoteToWinFast(State* states, int szStates) {
    int totEVs = totalEVs(states, szStates);
    int reqEVs = totEVs / 2 + 1; // required EVs to win election

    MinInfo** memo = (MinInfo**)malloc((szStates + 1) * sizeof(MinInfo*));
    for (int i = 0; i < szStates + 1; ++i) {
        memo[i] = (MinInfo*)malloc((reqEVs + 1) * sizeof(MinInfo));
        for (int j = 0; j < reqEVs + 1; ++j) {
            memo[i][j].subsetPVs = -1;
        }
    }
    MinInfo res = minPopVoteAtLeastFast(states, szStates, 0, reqEVs, memo);

    for (int i = 0; i < szStates + 1; ++i) {
        free(memo[i]);
    }
    free(memo);

    return res;

}

// Custom comparison function for sorting
int compareStates(const void* a, const void* b) {
    // Cast the parameters back to State pointers
    State* stateA = (State*)a;
    State* stateB = (State*)b;

    // Use strcmp to compare the names of the states
    return strcmp(stateA->name, stateB->name);
}

bool writeSubsetData(char* filenameW, int totEVs, int totPVs, int wonEVs, MinInfo toWin) {
    FILE* file = fopen(filenameW, "w");

    if (file == NULL) {
        return false; // Failed to open the file
    }

    qsort(toWin.someStates, toWin.szSomeStates, sizeof(State), compareStates);

    // Write the first line with the format: [TotalEVs],[TotalPVs],[EVsWon],[PVsWon]
    fprintf(file, "%d,%d,%d,%d\n", totEVs, totPVs, wonEVs, toWin.subsetPVs);

    // Write the individual State details for the subset of states
    for (int i = 0; i < toWin.szSomeStates; i++) {
        State state = toWin.someStates[i];
        fprintf(file, "%s,%s,%d,%d\n", state.name, state.postalCode, state.electoralVotes, state.popularVotes);
    }

    fclose(file);
    return true;
}
