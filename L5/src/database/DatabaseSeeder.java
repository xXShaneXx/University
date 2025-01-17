package database;

import org.mindrot.jbcrypt.BCrypt;
import utils.BCryptUtil;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.Statement;

public class DatabaseSeeder {
    public static void seed() {
        try (Connection conn = DatabaseConnection.getConnection()) {
            System.out.println("Initializing database...");

            try (Statement stmt = conn.createStatement()) {
                DeleteAllData(conn);

                // Tworzenie tabel
                stmt.execute("""
                        CREATE TABLE IF NOT EXISTS Applog (
                            message VARCHAR(255) NOT NULL,
                            time TIMESTAMP DEFAULT CURRENT_TIMESTAMP
                        )
                        """);

                stmt.execute("""
                        CREATE TABLE IF NOT EXISTS Roles (
                            id INT AUTO_INCREMENT PRIMARY KEY,
                            name VARCHAR(50) NOT NULL UNIQUE
                        )
                        """);

                stmt.execute("""
                        CREATE TABLE IF NOT EXISTS Users (
                            id INT AUTO_INCREMENT PRIMARY KEY,
                            username VARCHAR(50) NOT NULL UNIQUE,
                            email VARCHAR(100) NOT NULL UNIQUE,
                            password_hash VARCHAR(255) NOT NULL,
                            role_id INT NOT NULL,
                            FOREIGN KEY (role_id) REFERENCES Roles(id)
                        )
                        """);

                stmt.execute("""
                        CREATE TABLE IF NOT EXISTS Categories (
                            id INT AUTO_INCREMENT PRIMARY KEY,
                            name VARCHAR(100) NOT NULL
                        )
                        """);

                stmt.execute("""
                        CREATE TABLE IF NOT EXISTS Products (
                            id INT AUTO_INCREMENT PRIMARY KEY,
                            name VARCHAR(100) NOT NULL,
                            price DECIMAL(10, 2) NOT NULL,
                            category_id INT,
                            FOREIGN KEY (category_id) REFERENCES Categories(id)
                        )
                        """);

                stmt.execute("""
                        CREATE TABLE IF NOT EXISTS Orders (
                            id INT AUTO_INCREMENT PRIMARY KEY,
                            user_id INT,
                            order_date TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
                            total_quantity INT DEFAULT 0,
                            FOREIGN KEY (user_id) REFERENCES Users(id)
                        );

                        """);

                stmt.execute("""
                        CREATE TABLE IF NOT EXISTS OrderItems (
                            id INT AUTO_INCREMENT PRIMARY KEY,
                            order_id INT,
                            product_id INT,
                            quantity INT NOT NULL,
                            FOREIGN KEY (order_id) REFERENCES Orders(id),
                            FOREIGN KEY (product_id) REFERENCES Products(id)
                        )
                        """);

                System.out.println("Tables created successfully.");
            }

            // Seed data
            seedTriggers(conn);
            seedRoles(conn);
            seedCategories(conn);
            seedUsers(conn);
            seedProducts(conn);
            seedOrders(conn);
            seedOrderItems(conn);

        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private static void DeleteAllData(Connection conn) {
        try (Statement stmt = conn.createStatement()) {
            stmt.execute("DROP TABLE IF EXISTS Applog");
            stmt.execute("DROP TABLE IF EXISTS OrderItems");
            stmt.execute("DROP TABLE IF EXISTS Orders");
            stmt.execute("DROP TABLE IF EXISTS Products");
            stmt.execute("DROP TABLE IF EXISTS Categories");
            stmt.execute("DROP TABLE IF EXISTS Users");
            stmt.execute("DROP TABLE IF EXISTS Roles");



            ResultSet rs = stmt.executeQuery("SELECT TRIGGER_NAME FROM information_schema.TRIGGERS WHERE TRIGGER_SCHEMA = 'L5'");
            while (rs.next()) {
                String triggerName = rs.getString("TRIGGER_NAME");
                stmt.execute(String.format("DROP TRIGGER IF EXISTS %s", triggerName));
            }
            //System.out.println("All triggers deleted successfully.");

            System.out.println("All data deleted successfully.");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private static void seedRoles(Connection conn) {
        try (Statement stmt = conn.createStatement()) {
            //stmt.execute("DELETE FROM Users");
            //stmt.execute("DELETE FROM Roles");
            //stmt.execute("ALTER TABLE Roles AUTO_INCREMENT = 1;");

            String[] roles = {"Admin", "Customer"};
            for (String role : roles) {
                stmt.execute(String.format("INSERT INTO Roles (name) VALUES ('%s')", role));
            }
            System.out.println("Roles seeded successfully.");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private static void seedUsers(Connection conn) {
        try (Statement stmt = conn.createStatement()) {
            String adminPassword = "admin";
            String hashedAPassword = BCryptUtil.hashPassword(adminPassword);

            String testPassword = "test";
            String testHashedPassword = BCryptUtil.hashPassword(testPassword);

            Object[][] users = {
                    {"admin", "admin@example.com", hashedAPassword, 1}, // Admin role
                    {"john_doe", "john.doe@example.com", "$2a$12$userHashedPassword", 2}, // Customer role
                    {"test", "test@example.com", testHashedPassword, 2} // Customer role
            };

            for (Object[] user : users) {
                stmt.execute(String.format("""
                        INSERT INTO Users (username, email, password_hash, role_id)
                        VALUES ('%s', '%s', '%s', %d)
                        """, user[0], user[1], user[2], user[3]));
            }
            System.out.println("Users seeded successfully.");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private static void seedCategories(Connection conn) {
        try (Statement stmt = conn.createStatement()) {
            //stmt.execute("DELETE FROM Products");
            //stmt.execute("DELETE FROM Categories");
            //stmt.execute("ALTER TABLE Categories AUTO_INCREMENT = 1;");

            String[] categories = {"Electronics", "Clothing", "Home Appliances", "Books", "Toys"};
            for (String category : categories) {
                stmt.execute(String.format("INSERT INTO Categories (name) VALUES ('%s')", category));
            }
            System.out.println("Categories seeded successfully.");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private static void seedProducts(Connection conn) {
        try (Statement stmt = conn.createStatement()) {
            //stmt.execute("DELETE FROM Products");

            Object[][] products = {
                    {"Laptop", 1200.99, 1}, // Electronics
                    {"Smartphone", 800.50, 1}, // Electronics
                    {"T-shirt", 19.99, 2}, // Clothing
                    {"Jeans", 49.99, 2}, // Clothing
                    {"Vacuum Cleaner", 250.00, 3}, // Home Appliances
                    {"Cookbook", 15.99, 4}, // Books
                    {"Children''s Puzzle", 12.99, 5} // Toys
            };

            for (Object[] product : products) {
                stmt.execute(String.format("""
                    INSERT INTO Products (name, price, category_id)
                    VALUES ('%s', %.2f, %d)
                    """, product[0], product[1], product[2]));
            }
            System.out.println("Products seeded successfully.");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private static void seedOrders(Connection conn) {
        try (Statement stmt = conn.createStatement()) {
            //stmt.execute("DELETE FROM Orders");

            Object[][] orders = {
                    {1, "2023-10-01 10:00:00"},
                    {2, "2023-10-02 11:00:00"}
            };

            for (Object[] order : orders) {
                stmt.execute(String.format("""
                    INSERT INTO Orders (user_id, order_date)
                    VALUES (%d, '%s')
                    """, order[0], order[1]));
            }

            stmt.execute("""
                        UPDATE Orders
                        SET total_quantity = (
                            SELECT COALESCE(SUM(quantity), 0)
                            FROM OrderItems
                            WHERE OrderItems.order_id = Orders.id
                        );

                        """);
            System.out.println("Orders seeded successfully.");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private static void seedOrderItems(Connection conn) {
        try (Statement stmt = conn.createStatement()) {
            stmt.execute("DELETE FROM OrderItems");

            Object[][] orderItems = {
                    {1, 1, 2},
                    {1, 2, 1},
                    {2, 3, 1}
            };

            for (Object[] orderItem : orderItems) {
                stmt.execute(String.format("""
                    INSERT INTO OrderItems (order_id, product_id, quantity)
                    VALUES (%d, %d, %d)
                    """, orderItem[0], orderItem[1], orderItem[2]));
            }
            System.out.println("OrderItems seeded successfully.");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private static void seedTriggers(Connection conn) {
        try (Statement stmt = conn.createStatement()) {

            stmt.execute("""
            CREATE TRIGGER after_roles_update
            AFTER UPDATE ON Roles
            FOR EACH ROW
            BEGIN
                INSERT INTO Applog (message) VALUES (CONCAT('Role updated: ', OLD.name, ' to ', NEW.name));
            END;
            """);

            stmt.execute("""
            CREATE TRIGGER after_roles_delete
            AFTER DELETE ON Roles
            FOR EACH ROW
            BEGIN
                INSERT INTO Applog (message) VALUES (CONCAT('Role deleted: ', OLD.name));
            END;
            """);

            stmt.execute("""
            CREATE TRIGGER after_roles_insert
            AFTER INSERT ON Roles
            FOR EACH ROW
            BEGIN
                INSERT INTO Applog (message) VALUES (CONCAT('Role added: ', NEW.name));
            END;
            """);

            stmt.execute("""
            CREATE TRIGGER after_users_update
            AFTER UPDATE ON Users
            FOR EACH ROW
            BEGIN
                INSERT INTO Applog (message) VALUES (CONCAT('User updated: ', OLD.username, ' to ', NEW.username, ', Email: ', OLD.email, ' to ', NEW.email, ', Role ID: ', OLD.role_id, ' to ', NEW.role_id));
            END;
            """);

            stmt.execute("""
            CREATE TRIGGER after_users_insert
            AFTER INSERT ON Users
            FOR EACH ROW
            BEGIN
                INSERT INTO Applog (message) VALUES (CONCAT('User added: ', NEW.username, ', Email: ', NEW.email, ', Role ID: ', NEW.role_id));
            END;
            """);

            stmt.execute("""
            CREATE TRIGGER after_users_delete
            AFTER DELETE ON Users
            FOR EACH ROW
            BEGIN
                INSERT INTO Applog (message) VALUES (CONCAT('User deleted: ', OLD.username, ', Email: ', OLD.email, ', Role ID: ', OLD.role_id));
            END;
            """);

            stmt.execute("""
            CREATE TRIGGER after_products_update
            AFTER UPDATE ON Products
            FOR EACH ROW
            BEGIN
                INSERT INTO Applog (message) VALUES (CONCAT('Product updated: ', OLD.name, ' to ', NEW.name, ', Price: ', OLD.price, ' to ', NEW.price, ', Category ID: ', OLD.category_id, ' to ', NEW.category_id));
            END;
            """);

            stmt.execute("""
            CREATE TRIGGER after_products_insert
            AFTER INSERT ON Products
            FOR EACH ROW
            BEGIN
                INSERT INTO Applog (message) VALUES (CONCAT('Product added: ', NEW.name, ', Price: ', NEW.price, ', Category ID: ', NEW.category_id));
            END;
            """);

            stmt.execute("""
            CREATE TRIGGER after_products_delete
            AFTER DELETE ON Products
            FOR EACH ROW
            BEGIN
                INSERT INTO Applog (message) VALUES (CONCAT('Product deleted: ', OLD.name, ', Price: ', OLD.price, ', Category ID: ', OLD.category_id));
            END;
            """);

            stmt.execute("""
            CREATE TRIGGER after_order_item_delete
            AFTER DELETE ON OrderItems
            FOR EACH ROW
            BEGIN
                UPDATE Orders
                SET total_quantity = total_quantity - OLD.quantity
                WHERE id = OLD.order_id;
            END;
            """);

            stmt.execute("""
            CREATE TRIGGER after_order_item_update
            AFTER UPDATE ON OrderItems
            FOR EACH ROW
            BEGIN
                UPDATE Orders
                SET total_quantity = total_quantity - OLD.quantity + NEW.quantity
                WHERE id = NEW.order_id;
            END;
            """);



            System.out.println("Triggers created successfully.");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
