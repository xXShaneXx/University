import Product from "../models/productModel.js";

export const productMiddleware = async (req, res, next) => {
  try {
    const product = await Product.findById(req.params.id);

    if (!product) {
      return res.status(404).json({ message: "Product not found" });
    }

    // Jeśli admin — OK
    if (req.user.role === "admin") return next();

    // Jeśli właściciel — OK
    if (product.owner.toString() === req.user.id) return next();

    return res.status(403).json({ message: "You are not allowed to perform this action" });

  } catch (err) {
    console.error(err);
    res.status(500).json({ message: "Authorization error" });
  }
};
