import Product from "../models/productModel.js";
import Review from "../models/reviewModel.js";

export const addProduct = async (req, res) => {
    try {
        const { name, description, price } = req.body;
        const newProduct = new Product({
            name,
            description,
            price,
            owner: req.user.id // Pobieramy właściciela z tokena JWT
        });
        await newProduct.save();
        res.status(201).json({ message: `Product ${name} added successfully` });

    } catch (err) {
        console.error(err);
        res.status(500).json({ message: `Adding product went wrong` });
    }
};

export const deleteProduct = async (req, res) => {
    try {
        const product = await Product.findByIdAndDelete(req.params.id);
        if (!product) {
            return res.status(404).json({ message: `Product not found` });
        }
                
        // Delete all reviews for product
        await Review.deleteMany({ product: product._id });

        res.status(200).json({ message: `Product deleted successfully` });
    } catch (err) {
        console.error(err);
        res.status(500).json({ message: `Deleting product went wrong` });
    }
};

export const updateProduct = async (req, res) => {
    try {
        const { name, description, price } = req.body;
        const updatedProduct = await Product.findByIdAndUpdate(
            req.params.id,
            { name, description, price },
            { new: true }
        );

        if (!updatedProduct) {
            return res.status(404).json({ message: `Product not found` });
        }

        res.status(200).json(updatedProduct);
    } catch (err) {
        console.error(err);
        res.status(500).json({ message: `Updating product went wrong` });
    }
};

export const getProducts = async (req, res) => {
    try {
        const page = parseInt(req.query.page) || 1;
        const limit = parseInt(req.query.limit) || 10;
        const skip = (page - 1) * limit;

        const products = await Product.find()
            .skip(skip)
            .limit(limit);

        const total = await Product.countDocuments();

        res.status(200).json({
            total,
            page,
            pages: Math.ceil(total / limit),
            products
        });
    } catch (err) {
        console.error(err);
        res.status(500).json({ message: "Fetching products failed" });
    }
};

export const getProductById = async (req, res) => {
    try {
        const product = await Product.findById(req.params.id);
        if (!product) {
            return res.status(404).json({ message: `Product not found` });
        }
        res.status(200).json(product);
    } catch (err) {
        console.error(err);
        res.status(500).json({ message: `Fetching product went wrong` });
    }
};

export const getProductByName = async (req, res) => {
    try {
        const product = await Product.find({ name: req.params.name });
        if (!product) {
            return res.status(404).json({ message: `Product not found` });
        }
        res.status(200).json(product);
    } catch (err) {
        console.error(err);
        res.status(500).json({ message: `Fetching product went wrong` });
    }
};

export const getProductsByPrice = async (req, res) => {
    try {
        const { minPrice, maxPrice, sort } = req.query;
        const filter = {};

        if (minPrice) filter.price = { ...filter.price, $gte: Number(minPrice) };
        if (maxPrice) filter.price = { ...filter.price, $lte: Number(maxPrice) };

        // domyślnie sortowanie rosnąco, jeśli nie podano parametru sort
        let sortOrder = 1;
        if (sort === "desc") sortOrder = -1;
        if (sort === "asc") sortOrder = 1;

        const products = await Product.find(filter).sort({ price: sortOrder });
        res.status(200).json(products);
    } catch (err) {
        console.error(err);
        res.status(500).json({ message: `Fetching products by price went wrong` });
    }
};