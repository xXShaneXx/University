package models;

import java.util.ArrayList;
import java.util.List;

public class Order {
    private int id;
    private int userId;
    private String status;
    private List<OrderItem> orderItems = new ArrayList<>();

    // Getters and setters
    public int getId() {
        return id;
    }

    public int getUserId() {
        return userId;
    }

    public String getStatus() {
        return status;
    }

    public List<OrderItem> getOrderItems() {
        return orderItems;
    }

    public void setId(int id) {
        this.id = id;
    }

    public void setUserId(int userId) {
        this.userId = userId;
    }

    public void setStatus(String status) {
        this.status = status;
    }

    public void addOrderItem(OrderItem item) {
        orderItems.add(item);
    }
}

