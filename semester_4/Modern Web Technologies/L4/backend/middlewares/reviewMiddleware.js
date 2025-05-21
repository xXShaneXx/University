import Review from "../models/reviewModel.js";

export const canDeleteReview = async (req, res, next) => {
  try {
    const review = await Review.findById(req.params.id);

    if (!review) {
      return res.status(404).json({ message: "Review not found" });
    }

    if (req.user.role === "admin" || review.user.toString() === req.user.id) {
      return next();
    }

    return res.status(403).json({ message: "You are not allowed to delete this review" });
  } catch (err) {
    console.error(err);
    return res.status(500).json({ message: "Authorization failed" });
  }
};
