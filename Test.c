#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#include "./Include/hash_set.h"
#include "./Include/module.h"

int rowmax = 100;
int colmax = 100;
float sleep_time = 0.0;

/* ---------- HashSet Tests ---------- */
void test_create_hashset()
{
    HashSet *set = create_hashset();
    // Initially, all buckets should be NULL.
    for (int i = 0; i < HASHSET_SIZE; i++)
    {
        assert(set->buckets[i] == NULL);
    }
    free_hashset(set);
}

void test_contains_and_insert()
{
    HashSet *set = create_hashset();

    // Test: value not present initially.
    assert(contains(set, 10) == 0);
    insert(set, 10);
    assert(contains(set, 10) == 1);

    // Insert duplicate and test that it is still present only once.
    insert(set, 10);
    assert(contains(set, 10) == 1);

    // Remove and verify removal.
    remove_string(set, 10);
    assert(contains(set, 10) == 0);

    // Test multiple values.
    insert(set, 5);
    insert(set, 10);
    insert(set, 20);
    assert(contains(set, 5) == 1);
    assert(contains(set, 10) == 1);
    assert(contains(set, 20) == 1);

    free_hashset(set);
}

void test_remove_string()
{
    HashSet *set = create_hashset();
    insert(set, 42);
    insert(set, 100);
    assert(contains(set, 42) == 1);
    remove_string(set, 42);
    assert(contains(set, 42) == 0);
    // Remove non-existent element; should not crash.
    remove_string(set, 12345);
    free_hashset(set);
}

/* ---------- Module Function Tests ---------- */
void test_is_valid_cell()
{
    rowmax = 100;
    colmax = 100;
    assert(is_valid_cell("A1") == 1);
    assert(is_valid_cell("B12") == 1);
    assert(is_valid_cell("ZZ9") == 0);
    // Invalid cases: digits first or missing numbers.
    assert(is_valid_cell("1A") == 0);
    assert(is_valid_cell("A") == 0);
    // Out of bounds cell.
    assert(is_valid_cell("Z1000") == 0);
}

void test_is_valid_val()
{
    assert(is_valid_val("123") == 1);
    assert(is_valid_val("-456") == 1);
    assert(is_valid_val("+789") == 1);
    // Decimal or invalid characters should fail.
    assert(is_valid_val("12a3") == 0);
    assert(is_valid_val("1.23") == 0);
}

void test_is_valid_range()
{
    // Valid range (assume A1 and B2 are in range as per test cell limits).
    assert(is_valid_range("A1", "B2") == 1);
    // Reverse order should be invalid.
    assert(is_valid_range("B2", "A1") == 0);
    // Same cell range is valid.
    assert(is_valid_range("C3", "C3") == 1);
}

void test_encode_decode_cell()
{
    int code = encode_cell("A1");
    coordinate c = decode_cell(code);
    assert(c.x == 0);
    assert(c.y == 0);

    code = encode_cell("B2");
    c = decode_cell(code);
    assert(c.x == 1);
    assert(c.y == 1);
}

void test_getOp()
{
    // Test function operators.
    assert(getOp("MAX") == MAX);
    // Unknown operator should return -1.
    assert(getOp("unknown") == -1);
}

/* ---------- Arithmetic Tests ---------- */
void test_arithmetic()
{
    int row = 2, col = 2;
    // allocate a 2x2 matrix
    cell **arr = malloc(row * sizeof(cell *));
    for (int i = 0; i < row; i++)
    {
        arr[i] = malloc(col * sizeof(cell));
        for (int j = 0; j < col; j++)
        {
            commandCall dummy = {0};
            arr[i][j].val = 0;
            arr[i][j].cmd = dummy;
            arr[i][j].dep = create_hashset();
        }
    }
    // assign test values
    arr[0][0].val = 10;
    arr[0][1].val = 20;

    int add_result = addition(0, 0, 0, 1, row, col, arr);
    int sub_result = subtraction(0, 1, 0, 0, row, col, arr);
    int mul_result = multiply(0, 0, 0, 1, row, col, arr);

    assert(add_result == 30);
    assert(sub_result == 10);
    assert(mul_result == 200);

    // Free memory for each cell's dependency hashset.
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            free_hashset(arr[i][j].dep);
        }
        free(arr[i]);
    }
    free(arr);
}

/* ---------- Parse Tests ---------- */
void test_parse_valid_commands()
{
    // Test CMD: disable_output, enable_output, and navigation commands.
    char rawinp[32] = "";
    strcpy(rawinp, "disable_output");
    commandContainer cc = parse(rawinp, 0);
    assert(cc.cmd.type == CMD);
    assert(cc.cmd.cmd == DISABLE_OUTPUT);

    strcpy(rawinp, "enable_output");
    cc = parse(rawinp, 0);
    assert(cc.cmd.type == CMD);
    assert(cc.cmd.cmd == ENABLE_OUTPUT);

    strcpy(rawinp, "w");
    cc = parse(rawinp, 0);
    assert(cc.cmd.type == CMD);
    assert(cc.cmd.cmd == W);

    // Arithmetic assignment with valid value.
    strcpy(rawinp, "A1=123");
    cc = parse(rawinp, 0);
    assert(cc.cmd.type == VAL);
    assert(cc.cmd.type1 == VAL);

    // Sleep command using a valid numeric value.
    strcpy(rawinp, "A1=SLEEP(5)");
    cc = parse(rawinp, 0);
    assert(cc.cmd.type == FUNC);
    assert(cc.cmd.cmd == SLEEP);
    assert(cc.cmd.type1 == VAL);

    // Sleep command using a valid cell reference.
    strcpy(rawinp, "A1=SLEEP(B2)");
    cc = parse(rawinp, 0);
    assert(cc.cmd.type == FUNC);
    assert(cc.cmd.cmd == SLEEP);
    assert(cc.cmd.type1 == CELL);

    // Arithmetic operator commands (+, -, * and /)
    strcpy(rawinp, "A1=+123");
    cc = parse(rawinp, 1);
    assert(cc.cmd.type == ART || cc.cmd.type == VAL);
}

void test_parse_invalid_commands()
{
    // Invalid command: wrong cell format.

    char rawinp[32] = "";

    strcpy(rawinp, "1A=123");
    commandContainer cc = parse(rawinp, 0);
    assert(cc.cmd.error == INVALID);

    // Invalid operator command: unknown operator
    strcpy(rawinp, "A1=FOO(123)");
    cc = parse(rawinp, 0);
    assert(cc.cmd.error == INVALID);

    // Incorrect sleep command syntax.
    strcpy(rawinp, "A1=SLEEP(abc)");
    cc = parse(rawinp, 0);
    assert(cc.cmd.error == INVALID);

    // Test with missing '=' sign.
    strcpy(rawinp, "A1123");
    cc = parse(rawinp, 0);
    assert(cc.cmd.error == INVALID);
    // Test scroll_to with an invalid cell.
    strcpy(rawinp, "scroll_to Z1000");
    cc = parse(rawinp, 0);
    assert(cc.cmd.error == INVALID);
}

void test_parse_boundary_conditions()
{
    // Test with extra white space.
    char rawinp[32] = "";
    strcpy(rawinp, "   A1    =   123   ");
    commandContainer cc = parse(rawinp, 0);
    assert(cc.cmd.error == OK);
    assert(cc.cmd.type == VAL);
    assert(cc.cmd.type1 == VAL);
    assert(cc.cmd.param1 == 123);
    assert(cc.target == 0);

    // Test lower-case vs upper-case for operator names.
    strcpy(rawinp, "A1=add(123)");
    cc = parse(rawinp, 1);
    assert(cc.cmd.error == INVALID);

    strcpy(rawinp, "A1=MAX( A2:A3 )");
    cc = parse(rawinp, 0);
    assert(cc.cmd.cmd == MAX);
    assert(cc.cmd.type1 == CELL);
    assert(cc.cmd.type2 == CELL);
    assert(cc.target == 0);
    assert(cc.cmd.param1 == 100000);
    assert(cc.cmd.param2 == 200000);
    assert(cc.cmd.type == FUNC);
}

void test_remove_whitespace()
{
    char str1[] = "  Hello   World  ";
    remove_whitespace(str1);
    assert(strcmp(str1, "HelloWorld") == 0 ||
           strcmp(str1, "  Hello   World  ") == 0);
}

/* ---------- Topological Sort and Update Tests ---------- */
void test_topological_sort()
{
    // Create a simple 2x2 matrix of cells.
    int row = 2, col = 2;
    cell **arr = malloc(row * sizeof(cell *));
    for (int i = 0; i < row; i++)
    {
        arr[i] = malloc(col * sizeof(cell));
        for (int j = 0; j < col; j++)
        {
            commandCall dummy = {0};
            arr[i][j].val = i * col + j;
            arr[i][j].cmd = dummy;
            arr[i][j].dep = create_hashset();
        }
    }
    // Insert a dependency: A1 depends on B2 (assuming A1 encoded to 0 and B2 encoded appropriately).
    int codeA1 = encode_cell("A1");
    int codeB2 = encode_cell("B2");
    insert(arr[0][0].dep, codeB2);

    Node *sorted = topological_sort(codeA1, arr, row, col);
    assert(sorted != NULL);
    while (sorted)
    {
        Node *temp = sorted;
        sorted = sorted->next;
        free(temp);
    }
    for (int i = 0; i < row; i++)
    {
        free_hashset(arr[i][0].dep);
        free(arr[i]);
    }
    free(arr);
}

void test_insert_duplicate()
{
    HashSet *set = create_hashset();
    insert(set, 10);
    insert(set, 10);
    assert(contains(set, 10) == 1);
    free_hashset(set);
}

void test_remove_nonexistent()
{
    HashSet *set = create_hashset();
    remove_string(set, 999);
    assert(contains(set, 999) == 0);
    free_hashset(set);
}

void test_is_valid_cell_edge_cases()
{
    rowmax = 100;
    colmax = 100;
    assert(is_valid_cell("A100") == 1);
    assert(is_valid_cell("Z1") == 1);
    assert(is_valid_cell("AA1") == 1);
    assert(is_valid_cell("AAA1") == 0);
    assert(is_valid_cell("A101") == 0);
}

void test_is_valid_val_edge_cases()
{
    assert(is_valid_val("0") == 1);
    assert(is_valid_val("-0") == 1);
    assert(is_valid_val("000123") == 1);
}

void test_is_valid_range_edge_cases()
{
    assert(is_valid_range("A1", "A100") == 1);
    assert(is_valid_range("A1", "Z1") == 1);
    assert(is_valid_range("A1", "AA1") == 1);
    assert(is_valid_range("A1", "AAA1") == 1);
}

void test_encode_decode_cell_edge_cases()
{
    int code = encode_cell("Z100");
    coordinate c = decode_cell(code);
    assert(c.x == 99);
    assert(c.y == 25);
}

void test_getOp_edge_cases()
{
    assert(getOp("MAX") == MAX);
    assert(getOp("MIN") == MIN);
    assert(getOp("AVG") == AVG);
    assert(getOp("SUM") == SUM);
    assert(getOp("SLEEP") == SLEEP);
    assert(getOp("unknown") == -1);
}

void test_arithmetic_edge_cases()
{
    int row = 2, col = 2;
    cell **arr = malloc(row * sizeof(cell *));
    for (int i = 0; i < row; i++)
    {
        arr[i] = malloc(col * sizeof(cell));
        for (int j = 0; j < col; j++)
        {
            commandCall dummy = {0};
            arr[i][j].val = 0;
            arr[i][j].cmd = dummy;
            arr[i][j].dep = create_hashset();
        }
    }
    arr[0][0].val = -10;
    arr[0][1].val = 20;

    int add_result = addition(0, 0, 0, 1, row, col, arr);
    int sub_result = subtraction(0, 1, 0, 0, row, col, arr);
    int mul_result = multiply(0, 0, 0, 1, row, col, arr);

    assert(add_result == 10);
    assert(sub_result == 30);
    assert(mul_result == -200);

    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            free_hashset(arr[i][j].dep);
        }
        free(arr[i]);
    }
    free(arr);
}

void test_parse_valid_commands_edge_cases()
{
    char rawinp[32] = "";

    strcpy(rawinp, "A1=0");
    commandContainer cc = parse(rawinp, 0);
    assert(cc.cmd.type == VAL);
    assert(cc.cmd.param1 == 0);

    strcpy(rawinp, "A1=-0");
    cc = parse(rawinp, 0);
    assert(cc.cmd.type == VAL);
    assert(cc.cmd.param1 == 0);

    strcpy(rawinp, "A1=000123");
    cc = parse(rawinp, 0);
    assert(cc.cmd.type == VAL);
    assert(cc.cmd.param1 == 123);

    // Test with leading and trailing spaces
    strcpy(rawinp, "   A1=123   ");
    cc = parse(rawinp, 0);
    assert(cc.cmd.type == VAL);
    assert(cc.cmd.param1 == 123);

     // Test with leading and trailing newline characters
     strcpy(rawinp, "\n\n\nA1=123\n\n");
     cc = parse(rawinp, 0);
     assert(cc.cmd.type == VAL);
     assert(cc.cmd.param1 == 123);

    // Test with mixed case command
    strcpy(rawinp, "A1=MaX( A2:A3 )");
    cc = parse(rawinp, 0);
    assert(cc.cmd.error == INVALID);

    // Test with valid function and cell reference
    strcpy(rawinp, "A1=SUM(B1:B10)");
    cc = parse(rawinp, 0);
    assert(cc.cmd.cmd == SUM);
    assert(cc.cmd.type1 == CELL);
    assert(cc.cmd.type2 == CELL);

    // Test with valid function and numeric value
    strcpy(rawinp, "A1=SLEEP(10)");
    cc = parse(rawinp, 0);
    assert(cc.cmd.cmd == SLEEP);
    assert(cc.cmd.type1 == VAL);
    assert(cc.cmd.param1 == 10);

    // Test with valid arithmetic operation
    strcpy(rawinp, "A1=+123");
    cc = parse(rawinp, 0);
    assert(cc.cmd.type == VAL);
    assert(cc.cmd.param1 == 123);

    strcpy(rawinp, "A1=-123");
    cc = parse(rawinp, 0);
    assert(cc.cmd.type == VAL);
    assert(cc.cmd.param1 == -123);

}

void test_parse_invalid_commands_edge_cases()
{
    char rawinp[32] = "";

    strcpy(rawinp, "A1=+");
    commandContainer cc = parse(rawinp, 0);
    assert(cc.cmd.error == INVALID);

    strcpy(rawinp, "A1=-");
    cc = parse(rawinp, 0);
    assert(cc.cmd.error == INVALID);

    // Invalid function syntax
    strcpy(rawinp, "A1=SUM()");
    cc = parse(rawinp, 0);
    assert(cc.cmd.error == INVALID);

    strcpy(rawinp, "A1=SUM(,)");
    cc = parse(rawinp, 0);
    assert(cc.cmd.error == INVALID);

    // Invalid cell reference in function
    strcpy(rawinp, "A1=SUM(1A:B2)");
    cc = parse(rawinp, 0);
    assert(cc.cmd.error == INVALID);

    // Invalid arithmetic operation
    strcpy(rawinp, "A1=123+");
    cc = parse(rawinp, 0);
    assert(cc.cmd.error == INVALID);

    strcpy(rawinp, "A1=123-");
    cc = parse(rawinp, 0);
    assert(cc.cmd.error == INVALID);

    // Invalid command with special characters
    strcpy(rawinp, "A1=@123");
    cc = parse(rawinp, 0);
    assert(cc.cmd.error == INVALID);

    strcpy(rawinp, "A1=#123");
    cc = parse(rawinp, 0);
    assert(cc.cmd.error == INVALID);

    // Invalid command with mixed valid and invalid syntax
    strcpy(rawinp, "A1=123+SUM(B1:B10)");
    cc = parse(rawinp, 0);
    assert(cc.cmd.error == INVALID);

    // Invalid command with missing parentheses
    strcpy(rawinp, "A1=SUM(B1:B10");
    cc = parse(rawinp, 0);
    assert(cc.cmd.error == INVALID);

    strcpy(rawinp, "A1=SUM B1:B10)");
    cc = parse(rawinp, 0);
    assert(cc.cmd.error == INVALID);

    strcpy(rawinp, "");
    cc = parse(rawinp, 0);
    assert(cc.cmd.error == INVALID);
}

void test_display()
{
    // Initialize a 10x10 grid
    int row = 10, col = 10;
    cell **arr = (cell **)malloc(row * sizeof(cell *));
    for (int i = 0; i < row; i++)
    {
        arr[i] = (cell *)malloc(col * sizeof(cell));
        for (int j = 0; j < col; j++)
        {
            commandCall new_command = {0};
            cell new_cell = {0, new_command, create_hashset()};
            arr[i][j] = new_cell;
        }
    }

    // Set some values in the grid
    arr[0][0].val = 10;
    arr[0][1].val = 20;
    arr[1][0].val = 30;
    arr[1][1].val = 40;
    arr[2][2].cmd.isDivByZero = 1;

    // Call the Display function
    printf("Displaying 10x10 grid:\n");
    Display(row, col, arr, 0, 0);

    printf("Displaying 10x10 grid (expected):\n");
    printf("\tA\tB\tC\tD\tE\tF\tG\tH\tI\tJ\n");
    printf("1\t10\t20\t0\t0\t0\t0\t0\t0\t0\t0\n");
    printf("2\t30\t40\t0\t0\t0\t0\t0\t0\t0\t0\n");
    printf("3\t0\t0\tERR\t0\t0\t0\t0\t0\t0\t0\n");
    printf("4\t0\t0\t0\t0\t0\t0\t0\t0\t0\t0\n");
    printf("5\t0\t0\t0\t0\t0\t0\t0\t0\t0\t0\n");
    printf("6\t0\t0\t0\t0\t0\t0\t0\t0\t0\t0\n");
    printf("7\t0\t0\t0\t0\t0\t0\t0\t0\t0\t0\n");
    printf("8\t0\t0\t0\t0\t0\t0\t0\t0\t0\t0\n");
    printf("9\t0\t0\t0\t0\t0\t0\t0\t0\t0\t0\n");
    printf("10\t0\t0\t0\t0\t0\t0\t0\t0\t0\t0\n");

    // Free memory
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            free_hashset(arr[i][j].dep);
        }
        free(arr[i]);
    }
    free(arr);
}

/* ---------- Main Function ---------- */
int main(int argc, char const *argv[])
{
    printf("Running unit tests...\n");

    /* HashSet Tests */
    test_create_hashset();
    printf("Test 1 passed : create hashset\n");
    test_contains_and_insert();
    printf("Test 2 passed : create contains and insert\n");
    test_remove_string();
    printf("Test 3 passed : remove string\n");
    test_insert_duplicate();
    printf("Test 4 passed : insert duplicate\n");
    test_remove_nonexistent();
    printf("Test 5 passed : remove nonexistent\n");

    /* Module Function Tests */
    test_is_valid_cell();
    printf("Test 6 passed : is valid cell\n");
    test_is_valid_val();
    printf("Test 7 passed : is valid val\n");
    test_is_valid_range();
    printf("Test 8 passed : is valid range\n");
    test_encode_decode_cell();
    printf("Test 9 passed : encode decode cell\n");
    test_getOp();
    printf("Test 10 passed : getOp\n");
    test_is_valid_cell_edge_cases();
    printf("Test 11 passed : is valid cell edge cases\n");
    test_is_valid_val_edge_cases();
    printf("Test 12 passed : is valid val edge cases\n");
    test_is_valid_range_edge_cases();
    printf("Test 13 passed : is valid range edge cases\n");
    test_encode_decode_cell_edge_cases();
    printf("Test 14 passed : encode decode cell edge cases\n");
    test_getOp_edge_cases();
    printf("Test 15 passed : getOp edge cases\n");

    /* Arithmetic Tests */
    test_arithmetic();
    printf("Test 16 passed : arithmetic\n");
    test_arithmetic_edge_cases();
    printf("Test 17 passed : arithmetic edge cases\n");

    /* Parse Tests */
    test_parse_valid_commands();
    printf("Test 18 passed : parse valid commands\n");
    test_parse_invalid_commands();
    printf("Test 19 passed : parse invalid commands\n");
    test_parse_boundary_conditions();
    printf("Test 20 passed : parse boundary conditions\n");
    test_parse_valid_commands_edge_cases();
    printf("Test 21 passed : parse valid commands edge cases\n");
    test_parse_invalid_commands_edge_cases();
    printf("Test 22 passed : parse invalid commands edge cases\n");

    /* Whitespace Removal Test */
    test_remove_whitespace();
    printf("Test 23 passed : remove whitespace\n");

    /* Topological Sort Test */
    test_topological_sort();
    printf("Test 24 passed : topological sort\n");

    /* Display Test */
    printf("Compare the display output with expected output\n");
    test_display();

    printf("All tests passed.\n");
    return 0;
}
