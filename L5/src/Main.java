import database.DatabaseSeeder;
import models.User;
import org.mindrot.jbcrypt.BCrypt;
import services.AuthService;
import services.OrderService;
import services.ProductService;
import utils.ConsoleUtil;

public class Main {
    public static void main(String[] args) {
        DatabaseSeeder.seed();
        System.out.println(BCrypt.hashpw("admin", BCrypt.gensalt()));

        System.out.println("=== Welcome to the Store Management System ===");

        boolean running = true;
        while (running) {
            System.out.println("\nMenu:");
            System.out.println("1. Log in");
            System.out.println("2. Register");
            System.out.println("3. Exit");
            int choice = ConsoleUtil.readInt("Choose an option: ");

            switch (choice) {
                case 1 -> logInMenu();
                case 2 -> registerMenu();
                case 3 -> {
                    System.out.println("Exiting the application. Goodbye!");
                    running = false;
                }
                default -> System.out.println("Invalid option. Please try again.");
            }
        }

    }

    private static void registerMenu() {
        System.out.println("=== Register New User ===");
        String username = ConsoleUtil.readLine("Enter username: ");
        String email = ConsoleUtil.readLine("Enter email: ");
        String password = ConsoleUtil.readLine("Enter password: ");
        String confirmPassword = ConsoleUtil.readLine("Confirm password: ");

        if (!password.equals(confirmPassword)) {
            System.out.println("Passwords do not match. Try again.");
            return;
        }

        boolean success = AuthService.register(username, email, password);
        if (success) {
            System.out.println("Registration successful! You can now log in.");
        } else {
            System.out.println("Registration failed. Username or email might already be taken.");
        }
    }


    private static void logInMenu() {
        String username = ConsoleUtil.readLine("Enter username: ");
        String password = ConsoleUtil.readLine("Enter password: ");

        User user = AuthService.logIn(username, password);
        if (user != null) {
            System.out.println("Welcome, " + user.getUsername() + " (" + (user.getRoleId() == 1 ? "Admin" : "Customer") + ")!");

            if (user.getRoleId() == 1) {
                adminMenu();
            } else {
                customerMenu(user);
            }
        } else {
            System.out.println("Invalid credentials. Please try again.");
        }
    }

    private static void adminMenu() {
        boolean running = true;
        while (running) {
            System.out.println("\nAdmin Menu:");
            System.out.println("1. Manage Products");
            System.out.println("2. View Orders");
            System.out.println("3. Log Out");
            int choice = ConsoleUtil.readInt("Choose an option: ");

            switch (choice) {
                case 1 -> manageProductsMenu();
                case 2 -> OrderService.viewOrders();
                case 3 -> running = false;
                default -> System.out.println("Invalid option. Please try again.");
            }
        }
    }

    private static void manageProductsMenu() {
        boolean running = true;
        while (running) {
            System.out.println("\nProduct Management:");
            System.out.println("1. View Products");
            System.out.println("2. Add Product");
            System.out.println("3. Update Product");
            System.out.println("4. Delete Product");
            System.out.println("5. View Categories");
            System.out.println("6. Add Category");
            System.out.println("7. Update Category");
            System.out.println("8. Delete Category");
            System.out.println("9. Back");
            int choice = ConsoleUtil.readInt("Choose an option: ");

            switch (choice) {
                case 1 -> ProductService.viewProducts();
                case 2 -> ProductService.addProduct();
                case 3 -> ProductService.updateProduct();
                case 4 -> ProductService.deleteProduct();
                case 5 -> ProductService.viewCategories();
                case 6 -> ProductService.addCategory();
                case 7 -> ProductService.updateCategory();
                case 8 -> ProductService.deleteCategory();
                case 9 -> running = false;
                default -> System.out.println("Invalid option. Please try again.");
            }
        }
    }

    private static void customerMenu(User user) {
        boolean running = true;
        while (running) {
            System.out.println("\nCustomer Menu:");
            System.out.println("1. View Products");
            System.out.println("2. Place Order");
            System.out.println("3. View My Orders");
            System.out.println("4. Log Out");
            int choice = ConsoleUtil.readInt("Choose an option: ");

            switch (choice) {
                case 1 -> ProductService.viewProducts();
                case 2 -> OrderService.placeOrder(user);
                case 3 -> OrderService.viewOrdersForUser(user);
                case 4 -> running = false;
                default -> System.out.println("Invalid option. Please try again.");
            }
        }
    }
}
