// routes/reviewRoutes.js
import express from "express";
import { verifyToken } from "../middlewares/authMiddleware.js";
import {
  addReview,
  deleteReview,
  getReviewsByProduct
} from "../controllers/reviewController.js";
import { canDeleteReview } from "../middlewares/reviewMiddleware.js";

const router = express.Router();

// Dodaj recenzję (np. POST /api/products/:productId/reviews)
router.post("/:productId/reviews", verifyToken, addReview);

// Pobierz recenzje produktu (np. GET /api/products/:productId/reviews)
router.get("/:productId/reviews", getReviewsByProduct);

// Usuń recenzję (np. DELETE /api/products/reviews/:id)
router.delete("/reviews/:id", verifyToken, canDeleteReview, deleteReview);

export default router;
