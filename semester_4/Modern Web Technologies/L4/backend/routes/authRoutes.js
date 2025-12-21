import express from 'express'
import {registerUser, registerAdmin, login} from "../controllers/authController.js"
import { verifyToken } from "../middlewares/authMiddleware.js"
import { authorizeRoles } from "../middlewares/roleMiddleware.js"

const router = express.Router();

router.post("/register-user", registerUser);
// Only admin can register another admin
router.post("/register-admin", verifyToken, authorizeRoles("admin"), registerAdmin);

router.post("/login", login);

export default router;