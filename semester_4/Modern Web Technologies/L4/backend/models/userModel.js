import mongoose from "mongoose";

const userSchema = new mongoose.Schema({
    username: { 
        type: String, 
        required: true, 
        unique: true 
    },
    /*email: { 
        type: String, 
        required: true, 
        unique: true 
    }, */
    password: { 
        type: String, 
        required: true 
    },
    role: { 
        type: String, 
        required: true,
        enum: ["user", "admin"], 
        default: "user" 
    },
}, { timestamps: true }); //createdAt, updatedAt

const User = mongoose.model('User', userSchema);

export default User;