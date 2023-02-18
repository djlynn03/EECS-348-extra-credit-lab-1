#include <map>
#include <iostream>

using namespace std;

bool any_false(bool array[], int length)
// returns true if any elements in the array are false; returns false otherwise
{
    for (int i = 0; i < length; i++)
    {
        if (array[i] == false)
        {
            return true;
        }
    }
    return false;
}

int first_false(bool programmer_status[5])
// returns the index of the first element in the array that is false
{
    for (int i = 0; i < 5; i++)
    {
        if (programmer_status[i] == false)
            return i;
    }
    return 0;
}

void populate_data(int (&department_preferences)[5][5], int (&programmer_preferences)[5][5])
// populates the data for the departments and programmers from a file
{
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    size_t read;

    fp = fopen("input.txt", "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    int i = 0;
    while ((read = getline(&line, &len, fp)) != -1)
    {
        if (i < 5)
        {
            int l1 = 0;
            for (int j1 = 0; j1 < read; j1++)
            {
                if (isdigit(line[j1]))
                {
                    department_preferences[l1][i] = (line[j1] - '0');
                    l1++;
                }
            }
        }
        else if (i < 10)
        {
            int l2 = 0;
            for (size_t j2 = 0; j2 < read; j2++)
            {
                if (isdigit(line[j2]))
                {
                    programmer_preferences[l2][i - 5] = (line[j2] - '0');
                    l2++;
                }
            }
        }
        i++;
    }
    fclose(fp);
    if (line)
        free(line);
}

int main()
{
    int department_preferences[5][5];
    int programmer_preferences[5][5];

    populate_data(department_preferences, programmer_preferences);

    bool department_status[5] = {false};
    bool programmer_status[5] = {false};

    // maps to contain the assignments for each department and programmer
    map<int, int> department_assignment;
    map<int, int> programmer_assignment;

    // while there are unassigned programmers
    while (any_false(programmer_status, 5))
    {
        // try to assign the first unassigned programmer
        int programmer = first_false(programmer_status);

        int preferences[5];

        // load the programmer's preferences into an array
        for (int i = 0; i < 5; i++)
        {
            preferences[i] = programmer_preferences[programmer][i];
        }

        // for each preference, from highest to lowest priority
        for (int i : preferences)
        {
            // get department as 0-indexed int
            int department = i - 1;

            // if the department is not assigned, assign the programmer and department to each other
            if (!department_status[department])
            {
                programmer_assignment[programmer] = department;
                department_assignment[department] = programmer;
                programmer_status[programmer] = true;
                department_status[department] = true;
                break;
            }
            // if the department is already assigned
            else
            {
                // get the programmer assigned to the department
                int cur_programmer = department_assignment[department];
                int cur_preference = department_preferences[department][cur_programmer];
                // if the preference of the programmer is lower than the assigned department's programmer
                if (preferences[i - 1] < cur_preference)
                {
                    // reassign the current programmer and remove the programmer already assigned
                    programmer_assignment[programmer] = department;
                    department_assignment[department] = programmer;
                    programmer_status[programmer] = true;
                    programmer_status[cur_programmer] = false;
                    break;
                }
            }
        }
    }
    // print the results
    for (int i = 0; i < 5; i++)
    {
        printf("Department #%d will get programmer #%d\n", i + 1, department_assignment[i] + 1);
    }

    return 0;
}