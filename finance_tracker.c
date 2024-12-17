#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Constants
#define MAX_EXPENSES 1000
#define MAX_CATEGORIES 10
#define MAX_NAME_LENGTH 50
#define FILENAME "finance_data.txt"

// Expense Structure
typedef struct {
  char date[11];     // Date in YYYY-MM-DD format
  char category[50]; // Expense category
  float amount;      // Expense amount
} Expense;

// Global Variables
Expense expenses[MAX_EXPENSES];
int expense_count = 0;
float monthly_budget = 0;

// Function Prototypes
void add_expense();
void view_expenses();
void generate_report();
void set_budget();
void save_expenses();
void load_expenses();
float calculate_total_expenses();
float calculate_category_expenses(const char *category);

// Main Menu Function
void display_menu() {
  printf("\n=== Personal Finance Tracker ===\n");
  printf("1. Add Expense\n");
  printf("2. View Expenses\n");
  printf("3. Generate Financial Report\n");
  printf("4. Set Monthly Budget\n");
  printf("5. Save and Exit\n");
  printf("Enter your choice: ");
}

// Add Expense Function
void add_expense() {
  if (expense_count >= MAX_EXPENSES) {
    printf("Expense log is full. Cannot add more expenses.\n");
    return;
  }

  Expense new_expense;

  // Get Date
  printf("Enter date (YYYY-MM-DD): ");
  scanf("%10s", new_expense.date);

  // Get Category
  printf("Enter expense category: ");
  scanf("%49s", new_expense.category);

  // Get Amount
  printf("Enter expense amount: ");
  scanf("%f", &new_expense.amount);

  // Add to expenses array
  expenses[expense_count++] = new_expense;

  printf("Expense added successfully!\n");
}

// View Expenses Function
void view_expenses() {
  if (expense_count == 0) {
    printf("No expenses recorded.\n");
    return;
  }

  printf("\n=== Recorded Expenses ===\n");
  for (int i = 0; i < expense_count; i++) {
    printf("%d. Date: %s | Category: %s | Amount: $%.2f\n", i + 1,
           expenses[i].date, expenses[i].category, expenses[i].amount);
  }
}

// Generate Financial Report
void generate_report() {
  if (expense_count == 0) {
    printf("No expenses to generate report.\n");
    return;
  }

  float total_expenses = calculate_total_expenses();
  float remaining_budget = monthly_budget - total_expenses;

  printf("\n=== Financial Report ===\n");
  printf("Total Expenses: $%.2f\n", total_expenses);
  printf("Monthly Budget: $%.2f\n", monthly_budget);

  if (remaining_budget >= 0) {
    printf("Remaining Budget: $%.2f\n", remaining_budget);
    printf("Budget Status: ON TRACK\n");
  } else {
    printf("Budget Overrun: $%.2f\n", -remaining_budget);
    printf("Budget Status: EXCEEDED\n");
  }

  // Category-wise Expenses
  char *categories[] = {"Food", "Transport", "Utilities", "Entertainment",
                        "Others"};
  int num_categories = 5;

  printf("\n=== Category-wise Expenses ===\n");
  for (int i = 0; i < num_categories; i++) {
    float cat_expense = calculate_category_expenses(categories[i]);
    printf("%s: $%.2f (%.2f%%)\n", categories[i], cat_expense,
           (cat_expense / total_expenses) * 100);
  }
}

// Calculate Total Expenses
float calculate_total_expenses() {
  float total = 0;
  for (int i = 0; i < expense_count; i++) {
    total += expenses[i].amount;
  }
  return total;
}

// Calculate Expenses by Category
float calculate_category_expenses(const char *category) {
  float total = 0;
  for (int i = 0; i < expense_count; i++) {
    if (strcmp(expenses[i].category, category) == 0) {
      total += expenses[i].amount;
    }
  }
  return total;
}

// Set Monthly Budget
void set_budget() {
  printf("Enter monthly budget: $");
  scanf("%f", &monthly_budget);
  printf("Monthly budget set to $%.2f\n", monthly_budget);
}

// Save Expenses to File
void save_expenses() {
  FILE *file = fopen(FILENAME, "w");
  if (file == NULL) {
    printf("Error saving expenses.\n");
    return;
  }

  // Save budget first
  fprintf(file, "Budget:%.2f\n", monthly_budget);

  // Save expenses
  for (int i = 0; i < expense_count; i++) {
    fprintf(file, "%s,%s,%.2f\n", expenses[i].date, expenses[i].category,
            expenses[i].amount);
  }

  fclose(file);
  printf("Expenses saved successfully.\n");
}

// Load Expenses from File
void load_expenses() {
  FILE *file = fopen(FILENAME, "r");
  if (file == NULL) {
    printf("No previous data found.\n");
    return;
  }

  // Reset expense count
  expense_count = 0;

  // Read budget
  fscanf(file, "Budget:%f\n", &monthly_budget);

  // Read expenses
  while (fscanf(file, "%10[^,],%49[^,],%f\n", expenses[expense_count].date,
                expenses[expense_count].category,
                &expenses[expense_count].amount) == 3) {
    expense_count++;
    if (expense_count >= MAX_EXPENSES)
      break;
  }

  fclose(file);
  printf("Previous data loaded. %d expenses found.\n", expense_count);
}

// Main Function
int main() {
  // Load previous data
  load_expenses();

  int choice;
  do {
    display_menu();
    scanf("%d", &choice);

    switch (choice) {
    case 1:
      add_expense();
      break;
    case 2:
      view_expenses();
      break;
    case 3:
      generate_report();
      break;
    case 4:
      set_budget();
      break;
    case 5:
      save_expenses();
      printf("Exiting Personal Finance Tracker. Goodbye!\n");
      break;
    default:
      printf("Invalid choice. Please try again.\n");
    }
  } while (choice != 5);

  return 0;
}
