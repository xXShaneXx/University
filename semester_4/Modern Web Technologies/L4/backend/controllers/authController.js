import User from "../models/userModel.js"
import bcrypt from 'bcryptjs'
import jwt from 'jsonwebtoken'

export const registerUser = async (req, res) => {
    try {
        const {username, password} = req.body;
        const hashedPassword = await bcrypt.hash(password, 10);
    
        const newUser = new User({username, password: hashedPassword, role: "user"});
        await newUser.save(); //save to db
        res.status(201).json({message: `User registered with username ${username}`});
    } catch(err) {
        console.error(err);
        res.status(500).json({message: `registartion went wrong`});
    }
};

export const registerAdmin = async (req, res) => {
    try {
        const { username, password } = req.body;
        const hashedPassword = await bcrypt.hash(password, 10);
        const newUser = new User({ username, password: hashedPassword, role: "admin" });
        await newUser.save();
        res.status(201).json({ message: `Admin registered with username ${username}` });
    } catch (err) {
        console.error(err);
        res.status(500).json({ message: `admin registration went wrong` });
    }
};

export const login = async (req, res) => {
    try {
        const {username, password} = req.body;
        const user = await User.findOne({username});

        if(!user) {
            return res.status(404).json({message: `User ${username} not found`});
        }

        const isMatch = await bcrypt.compare(password, user.password);
        if(!isMatch) {
            return res.status(400).json({message: `Invalid credentails`});
        }

        const token = jwt.sign(
            {id: user._id, role: user.role}, 
            process.env.JWT_SECRET,
            {expiresIn: "1h"}
        );
        res.status(200).json({token});

    } catch(err) {
        res.status(500).json({message: `login went wrong`});
    }
};