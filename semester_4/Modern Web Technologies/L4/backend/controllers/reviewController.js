// controllers/reviewController.js
import Review from "../models/reviewModel.js";
import Product from "../models/productModel.js";

export const addReview = async (req, res) => {
  try {
    const { rating, comment } = req.body;
    const { productId } = req.params;

    const product = await Product.findById(productId);
    if (!product) {
      return res.status(404).json({ message: "Product not found" });
    }

    const newReview = new Review({
      rating,
      comment,
      product: productId,
      user: req.user.id
    });

    await newReview.save();
    res.status(201).json({ message: "Review added successfully" });
  } catch (err) {
    console.error(err);
    res.status(500).json({ message: "Adding review failed" });
  }
};

export const deleteReview = async (req, res) => {
  try {
    const review = await Review.findByIdAndDelete(req.params.id);
    if (!review) {
      return res.status(404).json({ message: "Review not found" });
    }
    res.status(200).json({ message: "Review deleted successfully" });
  } catch (err) {
    console.error(err);
    res.status(500).json({ message: "Deleting review failed" });
  }
};

export const getReviewsByProduct = async (req, res) => {
  try {
    const reviews = await Review.find({ product: req.params.productId }).populate("user", "username");
    res.status(200).json(reviews);
  } catch (err) {
    console.error(err);
    res.status(500).json({ message: "Fetching reviews failed" });
  }
};
