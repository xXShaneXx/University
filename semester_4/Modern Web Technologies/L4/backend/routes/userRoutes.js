import express from 'express'
import { verifyToken } from '../middlewares/authMiddleware.js';
import { authorizeRoles } from '../middlewares/roleMiddleware.js';
import { authorizeUser } from '../middlewares/userMiddleware.js';
import * as userController from '../controllers/userController.js';


const router = express.Router();

//Only admin can acces this router
router.get("/admin", verifyToken, authorizeRoles("admin"), (req, res) => {
    res.json({message: "Welcome Admin"});
});

//Only user and admin can acces this router
router.get("/user", verifyToken, authorizeRoles("admin", "user"), (req, res) => {
    res.json({message: "Welcome User"});
});

//Get all users(admins and users) only admins have premission
router.get('/all', verifyToken, authorizeRoles("admin"), userController.getAllUsers);

//Delete user only owner and admin have premissions
router.delete('/:id', verifyToken, authorizeUser, userController.deleteUser);

//Update user's data only owner and admin have premissions
router.put('/:id', verifyToken, authorizeUser, userController.updateUser);

export default router;