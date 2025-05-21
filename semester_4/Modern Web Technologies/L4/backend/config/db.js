import mongoose from "mongoose"

export const connectDB = async () => {
    try {
        const connection = await mongoose.connect(process.env.Mongo_URI)
        console.log("MongoDB connected");
    } catch(error) {
        console.error(`Error: ${error.message}`);
        process.exit(1); //Exit with failure
    }
}