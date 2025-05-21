import express from 'express'
import dotnev from 'dotenv'
import { connectDB } from './config/db.js'
import authRoutes from './routes/authRoutes.js'
import userRoutes from './routes/userRoutes.js'
import productRoutes from './routes/productRoutes.js'
import reviewRoutes from './routes/reviewRoutes.js';
import { verifyToken } from './middlewares/authMiddleware.js';

const app = express()
dotnev.config()

app.use(express.json()); //allows us to parse JSON data in the request body

app.use("/api/auth", authRoutes);
app.use("/api/users", userRoutes);
app.use("/api/products", productRoutes);
app.use("/api/products", reviewRoutes); // api/products/:productId/reviews


app.get('/', (req, res) => {
    res.send("Server is ready");
})

//for tests
app.get('/protected', verifyToken, (req, res) => {
    res.json({ message: 'You have access!', user: req.user });
});

app.listen(5000, () => {
    connectDB();
    console.log("server started at http://localhost:5000");
});

//Pawel
//1234