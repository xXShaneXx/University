import User from "../models/userModel.js"
import bcrypt from 'bcryptjs'

export const getAllUsers = async (req, res) => {
    try {
        const page = parseInt(req.query.page) || 1;
        const limit = parseInt(req.query.limit) || 10;
        const skip = (page - 1) * limit;

        const users = await User.find()
            .skip(skip)
            .limit(limit);

        const total = await User.countDocuments();

        res.status(200).json({
            total,
            page,
            pages: Math.ceil(total / limit),
            users
        });
    } catch (err) {
        console.error(err);
        res.status(500).json({ message: "Fetching users failed" });
    }
}

export const deleteUser = async (req, res) => {
    try {
        const user = await User.findByIdAndDelete(req.params.id);
        if (!user) {
            return res.status(404).json({ message: `User not found` });
        }
        res.status(200).json({ message: `User deleted successfully` });
    } catch (err) {
        console.error(err);
        res.status(500).json({ message: `Deleting user went wrong` });
    }
};

export const updateUser = async (req, res) => {
    try {
        const { username, password } = req.body;
        let updateData = { username };
        if (password) {
            updateData.password = await bcrypt.hash(password, 10);
        }
        const updatedUser = await User.findByIdAndUpdate(
            req.params.id,
            updateData,
            { new: true }
        );

        if (!updatedUser) {
            return res.status(404).json({ message: `User not found` });
        }

        res.status(200).json(updatedUser);
    } catch (err) {
        console.error(err);
        res.status(500).json({ message: `Updating user went wrong` });
    }
};