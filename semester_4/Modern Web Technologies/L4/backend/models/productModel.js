import mongoose from "mongoose";

const productSchema = new mongoose.Schema({
  name: {
    type: String,
    required: [true, "Nazwa produktu jest wymagana"]
  },
  description: {
    type: String,
    default: ""
  },
  price: {
    type: Number,
    required: [true, "Cena jest wymagana"],
    min: 0
  },
  owner: {
    type: mongoose.Schema.Types.ObjectId,
    ref: "User",
    required: true
  }
}, { timestamps: true });

const Product = mongoose.model("Product", productSchema);
export default Product;
