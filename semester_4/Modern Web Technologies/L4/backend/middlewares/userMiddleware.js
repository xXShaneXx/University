import User from "../models/userModel.js";

export const authorizeUser = async (req, res, next) => {
  try {
    const user = await User.findById(req.params.id);

    if (!user) {
      return res.status(404).json({ message: "user not found" });
    }

    // Jeśli admin — OK
    if (req.user.role === "admin") return next();

    // Jeśli właściciel — OK
    if (req.params.id === req.user.id) return next();

    return res.status(403).json({ message: "You are not allowed to perform this action" });

  } catch (err) {
    console.error(err);
    res.status(500).json({ message: "Authorization error" });
  }
};
