import express from 'express'
import dotnev from 'dotenv'
import cors from 'cors'
import { connectDB } from './config/db.js'
import authRoutes from './routes/authRoutes.js'
import userRoutes from './routes/userRoutes.js'
import productRoutes from './routes/productRoutes.js'
import reviewRoutes from './routes/reviewRoutes.js';
import { verifyToken } from './middlewares/authMiddleware.js';

dotnev.config()

const app = express()
const PORT = process.env.PORT || 5000

app.use(express.json()); //allows us to parse JSON data in the request body
app.use(cors({ origin: 'http://localhost:5173', credentials: true }));

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

app.listen(PORT, () => {
    connectDB();
    console.log("server started at http://localhost:" + PORT);
});

//Pawel
//1234