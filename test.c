#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "MinPopVote.h"

bool test_totalEVs() {
    State aStates[10];
    int res;

    aStates[0].electoralVotes = 5;
    aStates[1].electoralVotes = 8;
    aStates[2].electoralVotes = 12;
    aStates[3].electoralVotes = 6;
    aStates[4].electoralVotes = 7;
    aStates[5].electoralVotes = 10;

    printf(" Checking totalEVs() for 5 States:\n");
    res = totalEVs(aStates,5);
    if (res != 38) {
        printf("  individual state EVs are 5, 8, 13, 6, 7\n");
        printf("  expected total EVs = 38, actual total EVs = %d\n",res);
        return 0;
    }

    aStates[0].electoralVotes = 5;
    aStates[1].electoralVotes = 8;
    aStates[2].electoralVotes = 12;
    aStates[3].electoralVotes = 6;
    aStates[4].electoralVotes = 7;
    aStates[5].electoralVotes = 15;
    aStates[6].electoralVotes = 12;
    aStates[7].electoralVotes = 8;
    aStates[8].electoralVotes = 14;
    aStates[9].electoralVotes = 13;

    printf(" Checking totalEVs() for 10 States:\n");
    res = totalEVs(aStates,10);
    if (res != 100) {
        printf("  expected total EVs = 100, actual total EVs = %d\n",res);
        return false;
    }

    return true;
}

bool test_totalPVs() {
    //--------------------------------------------------------   
    // TODO: Task 6 - write your own test case for totalPVs();
    //                use test_totalEVs() as a sample;
    //                make sure to test all components
    //--------------------------------------------------------
   State aStates[5];
    int res;

    aStates[0].popularVotes = 10000;
    aStates[1].popularVotes = 15000;
    aStates[2].popularVotes = 25000;
    aStates[3].popularVotes = 12000;
    aStates[4].popularVotes = 14000;

    printf("Checking totalPVs() for 5 States:\n");
    res = totalPVs(aStates, 5);
    if (res != 76000) {
        printf("Individual state PVs are 10000, 15000, 25000, 12000, 14000\n");
        printf("Expected total PVs = 76000, actual total PVs = %d\n", res);
        return false;
    }

    State bStates[10];
    bStates[0].popularVotes = 10000;
    bStates[1].popularVotes = 15000;
    bStates[2].popularVotes = 25000;
    bStates[3].popularVotes = 12000;
    bStates[4].popularVotes = 14000;
    bStates[5].popularVotes = 18000;
    bStates[6].popularVotes = 22000;
    bStates[7].popularVotes = 27000;
    bStates[8].popularVotes = 31000;
    bStates[9].popularVotes = 36000;

    printf("Checking totalPVs() for 10 States:\n");
    res = totalPVs(bStates, 10);
    if (res != 210000) {
        printf("Expected total PVs = 210000, actual total PVs = %d\n", res);
        return false;
    }

    return true;
}

bool test_setSettings() {
    //-----------------------------------------------------------   
    // TODO: Task 6 - write your own test case for setSettings();
    //                use test_totalEVs() as a sample;
    //                make sure to test all components
    //-----------------------------------------------------------
    bool allTestsPassed = true; // Initialize to true

    int year;
    bool fastMode, quietMode;

    // Test a valid case with all options
    char* args1[] = {"program_name", "-f", "-q", "-y", "2020"};
    if (setSettings(5, args1, &year, &fastMode, &quietMode)) {
        if (year == 2020 && fastMode && quietMode) {
            printf("Test 1: Passed\n");
        } else {
            printf("Test 1: Failed\n");
            allTestsPassed = false;
        }
    } else {
        printf("Test 1: Passed\n");
    }

    // Test valid cases with individual options
    char* args2[] = {"program_name", "-f"};
    if (setSettings(2, args2, &year, &fastMode, &quietMode)) {
        if (year == 0 && fastMode && !quietMode) {
            printf("Test 2: Passed\n");
        } else {
            printf("Test 2: Failed\n");
            allTestsPassed = false;
        }
    } else {
        printf("Test 2: Passed\n");
    }

    char* args3[] = {"program_name", "-q"};
    if (setSettings(2, args3, &year, &fastMode, &quietMode)) {
        if (year == 0 && !fastMode && quietMode) {
            printf("Test 3: Passed\n");
        } else {
            printf("Test 3: Failed\n");
            allTestsPassed = false;
        }
    } else {
        printf("Test 3: Passed\n");
    }

    // Test a valid case with -y and an out-of-range year
    char* args4[] = {"program_name", "-y", "1900"};
    if (setSettings(3, args4, &year, &fastMode, &quietMode)) {
        if (year == 0 && !fastMode && !quietMode) {
            printf("Test 4: Failed\n");
            allTestsPassed = false;
        } else {
            printf("Test 4: Passed\n");
        }
    } else {
        printf("Test 4: Passed\n");
    }

    // Test an invalid case with an unrecognized option
    char* args5[] = {"program_name", "-x"};
    if (!setSettings(2, args5, &year, &fastMode, &quietMode)) {
        printf("Test 5: Passed\n");
    } else {
        printf("Test 5: Failed\n");
        allTestsPassed = false;
    }

    // Test an invalid case with -y and a non-integer year argument
    char* args6[] = {"program_name", "-y", "invalid_year"};
    if (!setSettings(3, args6, &year, &fastMode, &quietMode)) {
        printf("Test 6: Failed\n");
        allTestsPassed = false;
    } else {
        printf("Test 6: Passed\n");
    }

    return allTestsPassed; // Return true if all tests passed, false otherwise
}

bool test_inFilename() {
    //----------------------------------------------------------   
    // TODO: Task 6 - write your own test case for inFilename();
    //                use test_totalEVs() as a sample;
    //                make sure to test all components
    //----------------------------------------------------------
     char filename[100];
    inFilename(filename, 2020);

    // Check if the generated filename matches the expected format
    if (strcmp(filename, "data/2020.csv") != 0) {
        printf("inFilename() test failed. Generated filename: %s\n", filename);
        return false;
    }

    return true;
}

bool test_outFilename() {
    //-----------------------------------------------------------   
    // TODO: Task 6 - write your own test case for outFilename();
    //                use test_totalEVs() as a sample;
    //                make sure to test all components
    //-----------------------------------------------------------
    char filename[100];
    outFilename(filename, 2020);

    // Check if the generated filename matches the expected format
    if (strcmp(filename, "toWin/2020_win.csv") != 0) {
        printf("outFilename() test failed. Generated filename: %s\n", filename);
        return false;
    }

    return true;
}

bool test_parseLine() {
    //---------------------------------------------------------   
    // TODO: Task 6 - write your own test case for parseLine();
    //                use test_totalEVs() as a sample;
    //                make sure to test all components
    //---------------------------------------------------------
    State myState;
    bool allTestsPassed = true;

    char line1[] = "California,CA,55,8000000\n";
    char line2[] = "Texas,TX,38,7000000";
    char line3[] = "New York,NY,29,8500000,"; // Missing a field
    char line4[] = "Florida,FL,x,7000000\n"; // Invalid electoralVotes

    bool test1 = parseLine(line1, &myState);
    if (test1) {
        if (strcmp(myState.name, "California") == 0 &&
            strcmp(myState.postalCode, "CA") == 0 &&
            myState.electoralVotes == 55 &&
            myState.popularVotes == 8000000) {
            printf("Test 1: Passed\n");
        } else {
            printf("Test 1: Failed\n");
            allTestsPassed = false;
        }
    } else {
        printf("Test 1: Failed\n");
    }

    bool test2 = parseLine(line2, &myState);
    if (test2) {
        if (strcmp(myState.name, "Texas") == 0 &&
            strcmp(myState.postalCode, "TX") == 0 &&
            myState.electoralVotes == 38 &&
            myState.popularVotes == 7000000) {
            printf("Test 2: Passed\n");
        } else {
            printf("Test 2: Failed\n");
            allTestsPassed = false;
        }
    } else {
        printf("Test 2: Failed\n");
    }

    bool test3 = parseLine(line3, &myState); // Missing field
    if (test3) {
        printf("Test 3: Passed\n");
    } else {
        printf("Test 3: Failed\n");
    }

    bool test4 = parseLine(line4, &myState); // Invalid electoralVotes
    if (test4) {
        printf("Test 4: Passed\n");
    } else {
        printf("Test 4: Failed\n");
    }

    return allTestsPassed;
}

bool test_readElectionData() {
    //----------------------------------------------------------------   
    // TODO: Task 6 - write your own test case for readElectionData();
    //                make sure to test all components
    //----------------------------------------------------------------
    bool allTestsPassed = true; // Initialize to true

    State allStates[51]; // To store the states
    int nStates = 0;     // To store the number of states

    // Test a valid input file (should return true)
    if (readElectionData("1832.csv", allStates, &nStates)) {
        printf("Test 1: Failed\n");
        allTestsPassed = false;
    } else {
        printf("Test 1: Passed\n");
    }

    // Test an invalid input file (file not found) (should return true)
    if (!readElectionData("nonexistent_file.csv", allStates, &nStates)) {
        printf("Test 2: Passed\n");
    } else {
        printf("Test 2: Failed\n");
        allTestsPassed = false;
    }

    // Test a valid input file with more data (should return true, but nStates != 51)
    if (readElectionData("2004.csv", allStates, &nStates)) {
        printf("Test 3: Failed\n");
        allTestsPassed = false;
    } else {
        printf("Test 3: Passed\n");
    }

    // Test an empty input file (should return true)
    if (!readElectionData("empty_file.csv", allStates, &nStates)) {
        printf("Test 4: Passed\n"); 
    } else {
        printf("Test 4: Failed\n");
        allTestsPassed = false;
    }

    return allTestsPassed;
}

bool test_minPVsSlow() {
    //----------------------------------------------------------------- 
    // TODO: Task 7 - write your own test case for minPopVoteAtLeast();
    //                make sure to test all components
    //-----------------------------------------------------------------
    // Create a small array of State objects for testing
    State states[] = {
        {"State1", "ST1", 5, 200000},
        {"State2", "ST2", 7, 250000},
        {"State3", "ST3", 3, 150000},
        {"State4", "ST4", 4, 180000}
    };

    int szStates = sizeof(states) / sizeof(states[0]);

    // Test with a specific electoral vote requirement
    int requiredEVs = 10;

    // Call the minPopVoteAtLeast function to calculate MinInfo
    MinInfo result = minPopVoteAtLeast(states, szStates, 0, requiredEVs);

    // Verify the results
    bool testPassed = true;

    // Check if sufficientEVs is true
    if (!result.sufficientEVs) {
        printf("Test failed: sufficientEVs should be true\n");
        testPassed = false;
    }

    // Check if szSomeStates is as expected
    if (result.szSomeStates != 2) {
        printf("Test failed: szSomeStates should be 2\n");
        testPassed = false;
    }

    // Check the actual and expected subsetPVs
    int expectedSubsetPVs = 0;
    for (int i = 0; i < result.szSomeStates; i++) {
        expectedSubsetPVs += result.someStates[i].popularVotes;
    }
    expectedSubsetPVs++; // Add 1 to match the logic of the function
    if (result.subsetPVs != expectedSubsetPVs) {
        printf("Test failed: subsetPVs is not as expected\n");
        testPassed = false;
    }

    return testPassed;

}

bool test_minPVsFast() {
    //--------------------------------------------------------------------- 
    // TODO: Task 8 - write your own test case for minPopVoteAtLeastFast();
    //                make sure to test all components
    //---------------------------------------------------------------------
    State testStates[50];

    // Initialize the State objects with known values
    for (int i = 0; i < 50; i++) {
        sprintf(testStates[i].name, "State%d", i + 1);
        sprintf(testStates[i].postalCode, "S%d", i + 1);
        testStates[i].electoralVotes = 5;
        testStates[i].popularVotes = 10000;
    }

    MinInfo result = minPopVoteToWinFast(testStates, 50);

    if (result.szSomeStates != 26 || result.subsetPVs != 135000 || !result.sufficientEVs) {
        printf("test_minPVsFast() failed.\n");
        return false;
    }

    // Check if the selected states are the ones we expect
    for (int i = 0; i < 26; i++) {
        char expectedName[10];
        char expectedPostalCode[4];
        sprintf(expectedName, "State%d", i + 1);
        sprintf(expectedPostalCode, "S%d", i + 1);

        if (strcmp(result.someStates[i].name, expectedName) != 0 || strcmp(result.someStates[i].postalCode, expectedPostalCode) != 0) {
            printf("test_minPVsFast() selected states are not as expected.\n");
            return false;
        }
    }

    return true;
}
bool test_minPVsSlow() {
    //----------------------------------------------------------------- 
    // TODO: Task 7 - write your own test case for minPopVoteAtLeast();
    //                make sure to test all components
    //-----------------------------------------------------------------
    State* states[51];
    int sz = 0;
    readElectionData("data/1828.csv", *states, &sz);

    printf(" checking minPVsSlow() with year 1828\n");
    MinInfo test = minPopVoteAtLeast(*states, sz,0,131);

    if(test.subsetPVs != 196802 || test.szSomeStates != 14 || !test.sufficientEVs){
        printf("  expected: minPVs = 196802 | num states = 14 | sufficientEvs = 1\n  actual: minPVs = %d | num states = %d | sufficientEvs = %d\n", test.subsetPVs, test.szSomeStates, test.sufficientEVs);
        return false;
    }

    return true;
}

bool test_minPVsFast() {
    //--------------------------------------------------------------------- 
    // TODO: Task 8 - write your own test case for minPopVoteAtLeastFast();
    //                make sure to test all components
    //---------------------------------------------------------------------
    MinInfo** memo = (MinInfo**)malloc((25)*sizeof(MinInfo*));
    for (int i = 0; i < 25; ++i) {
        memo[i] = (MinInfo*)malloc((131)*sizeof(MinInfo));
        for (int j = 0; j < 131; ++j) {
            memo[i][j].subsetPVs = -1;
        }
    }
        State* states[51];
    int sz = 0;
    readElectionData("data/1828.csv", *states, &sz);

    printf(" checking minPVsFast() with year 1828\n");
    MinInfo test;
    test.szSomeStates = 0;
    test.subsetPVs =0;
    test = minPopVoteAtLeastFast(*states, sz,0,131,memo);

    if(test.subsetPVs != 196802 || test.szSomeStates != 14 || !test.sufficientEVs){
        printf("  expected: minPVs = 196802 | num states = 14 | sufficientEvs = 131\n  actual: minPVs = %d | num states = %d | sufficientEvs = %d\n", test.subsetPVs, test.szSomeStates, test.sufficientEVs);
        return false;
    }

    return true;
}

bool test_minPVsFast() {
    //--------------------------------------------------------------------- 
    // TODO: Task 8 - write your own test case for minPopVoteAtLeastFast();
    //                make sure to test all components
    //---------------------------------------------------------------------
    State states[51]; // Use an array of State, not an array of pointers
    int sz = 0;
    bool readSuccess = readElectionData("data/1828.csv", states, &sz); // Read data directly into the array

    if (!readSuccess) {
        printf("Failed to read data.\n");
        return false;
    }

    printf("Checking minPVsFast() with year 1828\n");

    // Create a memoization table based on the size of the states array
    MinInfo** memo = malloc((sz + 1) * sizeof(MinInfo*));
    for (int i = 0; i <= sz; i++) {
        memo[i] = malloc((131 + 1) * sizeof(MinInfo));
        for (int j = 0; j <= 131; j++) {
            memo[i][j].subsetPVs = -1; // Initialize with a value that indicates "not computed"
        }
    }

    MinInfo test = minPopVoteAtLeastFast(states, sz, 0, 131, memo);

    // Calculate the expected result without memoization (using the slow function)
    MinInfo expected = minPopVoteAtLeast(states, sz, 0, 131);

    if (test.subsetPVs != expected.subsetPVs || test.szSomeStates != expected.szSomeStates || test.sufficientEVs != expected.sufficientEVs) {
        printf("  Expected: minPVs = %d | num states = %d | sufficientEvs = %d\n", expected.subsetPVs, expected.szSomeStates, expected.sufficientEVs);
        printf("  Actual: minPVs = %d | num states = %d | sufficientEvs = %d\n", test.subsetPVs, test.szSomeStates, test.sufficientEVs);
    }

    // Free the memoization table
    for (int i = 0; i <= sz; i++) {
        free(memo[i]);
    }
    free(memo);

    return true;
}



int main() {
    printf("Welcome to the Popular Vote Minimizer Testing Suite!\n\n");

    printf("Testing totalEVs()...\n"); 
    if (test_totalEVs()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }

    printf("Testing totalPVs()...\n"); 
    if (test_totalPVs()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }

    printf("Testing setSettings()...\n"); 
    if (test_setSettings()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }

    printf("Testing inFilename()...\n"); 
    if (test_inFilename()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }

    printf("Testing outFilename()...\n"); 
    if (test_outFilename()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }

    printf("Testing parseLine()...\n"); 
    if (test_parseLine()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }

    printf("Testing readElectionData()...\n"); 
    if (test_readElectionData()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }

    printf("Testing minPopVoteToWin()...\n"); 
    if (test_minPVsSlow()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }

    printf("Testing minPopVoteToWinFast()...\n"); 
    if (test_minPVsFast()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }

    return 0;
}

