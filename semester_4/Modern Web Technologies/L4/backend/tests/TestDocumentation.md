# Test Documentation

## Authentication Tests

### Register a new user

* **Endpoint:** `POST http://localhost:5000/api/auth/register-user`
* **Method:** POST
* **Content-Type:** application/json
* **Body:**
    ```json
    {
      "username": "Test5",
      "password": "12345"
    }
    ```
* **Description:**
    * **Test Case:** Verify that a new user can be successfully registered in the system.
    * **Method:** Send a POST request to the register-user endpoint with valid user credentials (username and password).

### Login as user

* **Endpoint:** `POST http://localhost:5000/api/auth/login`
* **Method:** POST
* **Content-Type:** application/json
* **Body:**
    ```json
    {
      "username": "Test1",
      "password": "12345"
    }
    ```
* **Description:**
    * **Test Case:** Verify that an existing user can log in to the system.
    * **Method:** Send a POST request to the login endpoint with valid user credentials.

### Register a new admin (must be logged in as admin)

* **Endpoint:** `POST http://localhost:5000/api/auth/register-admin`
* **Method:** POST
* **Content-Type:** application/json
* **Authorization:** Bearer token (Admin)
* **Body:**
    ```json
    {
      "username": "TestAdmin",
      "password": "12345"
    }
    ```
* **Description:**
    * **Test Case:** Verify that a new admin user can be registered by an existing admin.
    * **Method:** Send a POST request to the register-admin endpoint with admin credentials in the request body and a valid admin token in the Authorization header.

### Login as admin

* **Endpoint:** `POST http://localhost:5000/api/auth/login`
* **Method:** POST
* **Content-Type:** application/json
* **Body:**
    ```json
    {
      "username": "TestAdmin",
      "password": "12345"
    }
    ```
* **Description:**
    * **Test Case:** Verify that an existing admin user can log in.
    * **Method:** Send a POST request to the login endpoint with valid admin credentials.

### Access protected route

* **Endpoint:** `GET http://localhost:5000/api/users/admin`
* **Method:** GET
* **Authorization:** Bearer token (Admin)
* **Description:**
    * **Test Case:** Verify that a protected route is only accessible to admin users.
    * **Method:** Send a GET request to a protected endpoint with a valid admin token.

### Access user route (user or admin)

* **Endpoint:** `GET http://localhost:5000/api/users/user`
* **Method:** GET
* **Authorization:** Bearer token (User or Admin)
* **Description:**
    * **Test Case:** Verify that a user route is accessible to both regular users and admins.
    * **Method:** Send a GET request to a user endpoint with a valid user or admin token.

### Edge Cases

### Register a new user (should succeed)

* **Endpoint:** `POST http://localhost:5000/api/auth/register-user`
* **Method:** POST
* **Content-Type:** application/json
* **Body:**
    ```json
    {
      "username": "TestEdge1",
      "password": "12345"
    }
    ```
* **Description:**
    * **Test Case:** Verify successful user registration with valid data.
    * **Method:** Send a POST request with valid username and password.

### Register with existing username (should fail)

* **Endpoint:** `POST http://localhost:5000/api/auth/register-user`
* **Method:** POST
* **Content-Type:** application/json
* **Body:**
    ```json
    {
      "username": "Test1",
      "password": "anotherpass"
    }
    ```
* **Description:**
    * **Test Case:** Verify that the system prevents registering a user with a username that already exists.
    * **Method:** Send a POST request with an existing username.

### Register user with missing password (should fail)

* **Endpoint:** `POST http://localhost:5000/api/auth/register-user`
* **Method:** POST
* **Content-Type:** application/json
* **Body:**
    ```json
    {
      "username": "TestEdge2"
    }
    ```
* **Description:**
    * **Test Case:** Verify that the system rejects user registration attempts with a missing password.
    * **Method:** Send a POST request with a username but without a password.

### Register user with missing username (should fail)

* **Endpoint:** `POST http://localhost:5000/api/auth/register-user`
* **Method:** POST
* **Content-Type:** application/json
* **Body:**
    ```json
    {
      "password": "12345"
    }
    ```
* **Description:**
    * **Test Case:** Verify that the system rejects user registration attempts with a missing username.
    * **Method:** Send a POST request with a password but without a username.

### Login with correct credentials (should succeed)

* **Endpoint:** `POST http://localhost:5000/api/auth/login`
* **Method:** POST
* **Content-Type:** application/json
* **Body:**
    ```json
    {
      "username": "TestEdge1",
      "password": "12345"
    }
    ```
* **Description:**
    * **Test Case:** Verify successful login with valid credentials.
    * **Method:** Send a POST request with a valid username and password.

### Login with wrong password (should fail)

* **Endpoint:** `POST http://localhost:5000/api/auth/login`
* **Method:** POST
* **Content-Type:** application/json
* **Body:**
    ```json
    {
      "username": "Test1",
      "password": "wrongpass"
    }
    ```
* **Description:**
    * **Test Case:** Verify that the system rejects login attempts with an incorrect password.
    * **Method:** Send a POST request with a valid username and an incorrect password.

### Login with non-existent user (should fail)

* **Endpoint:** `POST http://localhost:5000/api/auth/login`
* **Method:** POST
* **Content-Type:** application/json
* **Body:**
    ```json
    {
      "username": "NonExistentUser",
      "password": "whatever"
    }
    ```
* **Description:**
    * **Test Case:** Verify that the system rejects login attempts with a non-existent username.
    * **Method:** Send a POST request with a non-existent username.

### Register admin without token (should fail)

* **Endpoint:** `POST http://localhost:5000/api/auth/register-admin`
* **Method:** POST
* **Content-Type:** application/json
* **Body:**
    ```json
    {
      "username": "EdgeAdmin",
      "password": "adminpass"
    }
    ```
* **Description:**
    * **Test Case:** Verify that the system prevents registering an admin user without a valid admin token.
    * **Method:** Send a POST request to the register-admin endpoint without an Authorization header.

### Register admin with user token (should fail)

* **Endpoint:** `POST http://localhost:5000/api/auth/register-admin`
* **Method:** POST
* **Content-Type:** application/json
* **Authorization:** Bearer token (User)
* **Body:**
    ```json
    {
      "username": "EdgeAdmin2",
      "password": "adminpass"
    }
    ```
* **Description:**
    * **Test Case:** Verify that the system prevents registering an admin user with a regular user's token.
    * **Method:** Send a POST request to the register-admin endpoint with a regular user's token in the Authorization header.

### Register admin with admin token (should succeed)

* **Endpoint:** `POST http://localhost:5000/api/auth/register-admin`
* **Method:** POST
* **Content-Type:** application/json
* **Authorization:** Bearer token (Admin)
* **Body:**
    ```json
    {
      "username": "EdgeAdmin3",
      "password": "adminpass"
    }
    ```
* **Description:**
    * **Test Case:** Verify that an admin user can be successfully registered by another admin.
    * **Method:** Send a POST request to the register-admin endpoint with a valid admin token.

### Login as new admin (should succeed)

* **Endpoint:** `POST http://localhost:5000/api/auth/login`
* **Method:** POST
* **Content-Type:** application/json
* **Body:**
    ```json
    {
      "username": "EdgeAdmin3",
      "password": "adminpass"
    }
    ```
* **Description:**
    * **Test Case:** Verify that a newly registered admin can log in successfully.
    * **Method:** Send a POST request to the login endpoint with the new admin's credentials.

## Product Tests

### Log in to obtain token

* **Endpoint:** `POST http://localhost:5000/api/auth/login`
* **Method:** POST
* **Content-Type:** application/json
* **Body:**
    ```json
    {
      "username": "Test2",
      "password": "12345"
    }
    ```
* **Description:**
    * **Test Case:** Obtain a valid user token for subsequent product-related operations.
    * **Method:** Send a POST request to the login endpoint with user credentials.

### Add a new product (user or admin)

* **Endpoint:** `POST http://localhost:5000/api/products`
* **Method:** POST
* **Content-Type:** application/json
* **Authorization:** Bearer token (User or Admin)
* **Body:**
    ```json
    {
      "name": "To delete",
      "description": "Delete Object",
      "price": 0.99
    }
    ```
* **Description:**
    * **Test Case:** Verify that a user or an admin can add a new product.
    * **Method:** Send a POST request to the products endpoint with product details and a valid user or admin token.

### Get all products

* **Endpoint:** `GET http://localhost:5000/api/products`
* **Method:** GET
* **Description:**
    * **Test Case:** Verify that all products can be retrieved.
    * **Method:** Send a GET request to the products endpoint.

### Get product by ID

* **Endpoint:** `GET http://localhost:5000/api/products/<PRODUCT_ID>`
* **Method:** GET
* **Description:**
    * **Test Case:** Verify that a product can be retrieved by its ID.
    * **Method:** Send a GET request to the products endpoint with a specific product ID.

### Get product by name

* **Endpoint:** `GET http://localhost:5000/api/products/search/Test%20Product`
* **Method:** GET
* **Description:**
    * **Test Case:** Verify that products can be searched by name.
    * **Method:** Send a GET request to the products/search endpoint with a product name as a query parameter.

### Update product (owner or admin)

* **Endpoint:** `PUT http://localhost:5000/api/products/6829f31c4f87ca2f82a53abe`
* **Method:** PUT
* **Content-Type:** application/json
* **Authorization:** Bearer token (Owner or Admin)
* **Body:**
    ```json
    {
      "name": "Updated Product",
      "description": "Updated description",
      "price": 149.99
    }
    ```
* **Description:**
    * **Test Case:** Verify that a product can be updated by its owner or an admin.
    * **Method:** Send a PUT request to the products endpoint with the product ID and updated details, including a valid owner or admin token.

### Delete product (owner or admin)

* **Endpoint:** `DELETE http://localhost:5000/api/products/682a3283c33058ffeb1ba975`
* **Method:** DELETE
* **Authorization:** Bearer token (Owner or Admin)
* **Description:**
    * **Test Case:** Verify that a product can be deleted by its owner or an admin.
    * **Method:** Send a DELETE request to the products endpoint with the product ID and a valid owner or admin token.

### Filter products by price (default sort)

* **Endpoint:** `GET http://localhost:5000/api/products/filter/price`
* **Method:** GET
* **Description:**
    * **Test Case:** Verify that products can be filtered by price with the default sorting order.
    * **Method:** Send a GET request to the products/filter/price endpoint.

### Filter products by price (sort asc)

* **Endpoint:** `GET http://localhost:5000/api/products/filter/price?sort=asc`
* **Method:** GET
* **Description:**
    * **Test Case:** Verify that products can be filtered by price and sorted in ascending order.
    * **Method:** Send a GET request to the products/filter/price endpoint with the `sort=asc` query parameter.

### Filter products by price (sort desc)

* **Endpoint:** `GET http://localhost:5000/api/products/filter/price?sort=desc`
* **Method:** GET
* **Description:**
    * **Test Case:** Verify that products can be filtered by price and sorted in descending order.
    * **Method:** Send a GET request to the products/filter/price endpoint with the `sort=desc` query parameter.

### Filter products by price with minimum price and descending sort

* **Endpoint:** `GET http://localhost:5000/api/products/filter/price?minPrice=100&sort=desc`
* **Method:** GET
* **Description:**
    * **Test Case:** Verify that products can be filtered by price with a minimum price and sorted in descending order.
    * **Method:** Send a GET request to the products/filter/price endpoint with `minPrice` and `sort=desc` query parameters.

### Add product without token (should fail)

* **Endpoint:** `POST http://localhost:5000/api/products`
* **Method:** POST
* **Content-Type:** application/json
* **Body:**
    ```json
    {
      "name": "NoToken",
      "description": "Should fail",
      "price": 1.23
    }
    ```
* **Description:**
    * **Test Case:** Verify that adding a product without a token fails.
    * **Method:** Send a POST request to the products endpoint without an Authorization header.

### Add product with missing name (should fail)

* **Endpoint:** `POST http://localhost:5000/api/products`
* **Method:** POST
* **Content-Type:** application/json
* **Authorization:** Bearer token
* **Body:**
    ```json
    {
      "description": "No name",
      "price": 1.23
    }
    ```
* **Description:**
    * **Test Case:** Verify that adding a product with a missing name fails.
    * **Method:** Send a POST request to the products endpoint with product details but without a name.

### Edge Cases

### Add product with negative price (should fail)

* **Endpoint:** `POST http://localhost:5000/api/products`
* **Method:** POST
* **Content-Type:** application/json
* **Authorization:** Bearer token
* **Body:**
    ```json
    {
      "name": "NegativePrice",
      "description": "Should fail",
      "price": -10
    }
    ```
* **Description:**
    * **Test Case:** Verify that adding a product with a negative price fails.
    * **Method:** Send a POST request to the products endpoint with a negative price value.

### Get product by invalid ID (should fail)

* **Endpoint:** `GET http://localhost:5000/api

## Review Tests

### Log in to obtain token

* **Endpoint:** `POST http://localhost:5000/api/auth/login`
* **Method:** POST
* **Content-Type:** application/json
* **Body:**

    ```json
    {
      "username": "Test3",
      "password": "12345"
    }
    ```
* **Description:**
    * **Test Case:** Obtain a valid user token to authorize review-related operations.
    * **Method:** Send a POST request to the login endpoint with user credentials. [cite: 7]

### Get all products

* **Endpoint:** `GET http://localhost:5000/api/products`
* **Method:** GET
* **Description:**
    * **Test Case:** Retrieve a list of all products (this might be necessary to get a product ID for review tests).
    * **Method:** Send a GET request to the products endpoint. [cite: 7]

### Add a review to a product (user must be logged in)

* **Endpoint:** `POST http://localhost:5000/api/products/682a321ec33058ffeb1ba972/reviews`
* **Method:** POST
* **Content-Type:** application/json
* **Authorization:** Bearer token (User)
* **Body:**

    ```json
    {
      "rating": 5,
      "comment": "Review to delete!"
    }
    ```
* **Description:**
    * **Test Case:** Verify that a logged-in user can add a review to a product.
    * **Method:** Send a POST request to the `/api/products/{productId}/reviews` endpoint with the review details (rating and comment) and a valid user token in the Authorization header. [cite: 7, 8]

### Get all reviews for a product

* **Endpoint:** `GET http://localhost:5000/api/products/682a321ec33058ffeb1ba972/reviews`
* **Method:** GET
* **Description:**
    * **Test Case:** Verify that all reviews for a specific product can be retrieved.
    * **Method:** Send a GET request to the `/api/products/{productId}/reviews` endpoint. [cite: 8]

### Delete a review (owner or admin)

* **Endpoint:** `DELETE http://localhost:5000/api/products/reviews/682a34e1c33058ffeb1ba989`
* **Method:** DELETE
* **Authorization:** Bearer token (Owner or Admin)
* **Description:**
    * **Test Case:** Verify that a review can be deleted by its owner or an admin.
    * **Method:** Send a DELETE request to the `/api/products/reviews/{reviewId}` endpoint with a valid user or admin token in the Authorization header. [cite: 8]

### Add review without token (should fail)

* **Endpoint:** `POST http://localhost:5000/api/products/682a321ec33058ffeb1ba972/reviews`
* **Method:** POST
* **Content-Type:** application/json
* **Body:**

    ```json
    {
      "rating": 4,
      "comment": "Should fail, no token"
    }
    ```
* **Description:**
    * **Test Case:** Verify that adding a review without an authorization token fails.
    * **Method:** Send a POST request to the reviews endpoint without an Authorization header. [cite: 8]

### Add review with missing rating (should fail)

* **Endpoint:** `POST http://localhost:5000/api/products/682a321ec33058ffeb1ba972/reviews`
* **Method:** POST
* **Content-Type:** application/json
* **Authorization:** Bearer token (User)
* **Body:**

    ```json
    {
      "comment": "No rating"
    }
    ```
* **Description:**
    * **Test Case:** Verify that adding a review with a missing rating fails.
    * **Method:** Send a POST request to the reviews endpoint with a comment but without a rating. [cite: 8]

### Add review with invalid productId (should fail)

* **Endpoint:** `POST http://localhost:5000/api/products/invalidid/reviews`
* **Method:** POST
* **Content-Type:** application/json
* **Authorization:** Bearer token (User)
* **Body:**

    ```json
    {
      "rating": 5,
      "comment": "Invalid productId"
    }
    ```
* **Description:**
    * **Test Case:** Verify that adding a review with an invalid product ID fails.
    * **Method:** Send a POST request to the reviews endpoint with an invalid product ID in the URL. [cite: 8]

### Get reviews for non-existent product (should return empty or 404)

* **Endpoint:** `GET http://localhost:5000/api/products/000000000000000000000000/reviews`
* **Method:** GET
* **Description:**
    * **Test Case:** Verify the behavior when trying to get reviews for a non-existent product. The expected result could be an empty response or a 404 error.
    * **Method:** Send a GET request to the reviews endpoint with a non-existent product ID. [cite: 8]

### Delete review without token (should fail)

* **Endpoint:** `DELETE http://localhost:5000/api/products/reviews/682a34e1c33058ffeb1ba989`
* **Method:** DELETE
* **Description:**
    * **Test Case:** Verify that deleting a review without an authorization token fails.
    * **Method:** Send a DELETE request to the delete review endpoint without an Authorization header. [cite: 8]

### Delete review as not owner and not admin (should fail)

* **Endpoint:** `DELETE http://localhost:5000/api/products/reviews/682a34e1c33058ffeb1ba989`
* **Method:** DELETE
* **Authorization:** Bearer token (Not Owner)
* **Description:**
    * **Test Case:** Verify that deleting a review by a user who is neither the review owner nor an admin fails.
    * **Method:** Send a DELETE request to the delete review endpoint with a user token that does not belong to the review's author and is not an admin token. [cite: 8]

### Delete review with invalid reviewId (should fail)

* **Endpoint:** `DELETE http://localhost:5000/api/products/reviews/invalidid`
* **Method:** DELETE
* **Authorization:** Bearer token (User)
* **Description:**
    * **Test Case:** Verify that deleting a review with an invalid review ID fails.
    * **Method:** Send a DELETE request to the delete review endpoint with an invalid review ID. [cite: 9]