package models;

public class OrderItem {
    private int productId;
    private int quantity;

    // Getters and setters

    public int getProductId() {
        return productId;
    }
    
    public int getQuantity() {
        return quantity;
    }
    
    public void setProductId(int productId) {
        this.productId = productId;
    }
    
    public void setQuantity(int quantity) {
        this.quantity = quantity;
    }
}
