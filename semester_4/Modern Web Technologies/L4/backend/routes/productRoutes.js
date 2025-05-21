// routes/productRoutes.js
import express from 'express';
import {
  addProduct,
  deleteProduct,
  updateProduct,
  getProducts,
  getProductById,
  getProductByName,
  getProductsByPrice
} from '../controllers/productController.js';

import { verifyToken } from '../middlewares/authMiddleware.js';
import { authorizeRoles } from '../middlewares/roleMiddleware.js';
import { productMiddleware } from '../middlewares/productMiddleware.js';

const router = express.Router();

// Dodaj produkt (tylko zalogowany użytkownik)
router.post("/", verifyToken, authorizeRoles("admin", "user"), addProduct);

// Usuń produkt (właściciel lub admin)
router.delete("/:id", verifyToken, productMiddleware , deleteProduct);

// Edytuj produkt (właściciel lub admin)
router.put("/:id", verifyToken, productMiddleware , updateProduct);

// Pobierz wszystkie produkty
router.get("/", getProducts);

// Pobierz produkt po ID
router.get("/:id", getProductById);

// Pobierz produkt po nazwie
router.get("/search/:name", getProductByName);

//filtrowanie po cenie
router.get('/filter/price', getProductsByPrice);

export default router;
