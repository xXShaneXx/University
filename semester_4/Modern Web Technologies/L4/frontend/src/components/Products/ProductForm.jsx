import React, { useState, useEffect, useContext } from 'react';
import { useNavigate, useParams } from 'react-router-dom';
import { addProduct, updateProduct, getProductById } from '../../services/productService';
import { AuthContext } from '../../context/AuthContext';
import '../../assets/ProductForm.css';

const ProductForm = () => {
  const { id } = useParams(); // If editing, id will be present
  const navigate = useNavigate();
  const { user } = useContext(AuthContext);

  const [form, setForm] = useState({
    name: '',
    price: '',
    description: ''
  });
  const [loading, setLoading] = useState(false);
  const [error, setError] = useState('');

  // If editing, fetch product data
  useEffect(() => {
    if (id) {
      setLoading(true);
      getProductById(id)
        .then(product => {
          setForm({
            name: product.name,
            price: product.price,
            description: product.description
          });
        })
        .catch(() => setError('Failed to load product'))
        .finally(() => setLoading(false));
    }
  }, [id]);

  const handleChange = (e) => {
    setForm({ ...form, [e.target.name]: e.target.value });
  };

  const handleSubmit = async (e) => {
    e.preventDefault();
    setError('');
    setLoading(true);
    try {
      if (id) {
        await updateProduct(id, form);
      } else {
        await addProduct(form);
      }
      navigate('/products');
    } catch (err) {
      setError('Failed to save product');
    } finally {
      setLoading(false);
    }
  };

  if (!user) return <div>You must be logged in to add or edit products.</div>;
  if (loading) return <div>Loading...</div>;

  return (
    <form onSubmit={handleSubmit} className="product-form">
      <h2>{id ? 'Edit Product' : 'Add Product'}</h2>
      {error && <div style={{ color: 'red' }}>{error}</div>}
      <div>
        <label>Name:</label>
        <input
          name="name"
          value={form.name}
          onChange={handleChange}
          required
        />
      </div>
      <div>
        <label>Price:</label>
        <input
          name="price"
          type="number"
          min="0"
          step="0.01"
          value={form.price}
          onChange={handleChange}
          required
        />
      </div>
      <div>
        <label>Description:</label>
        <textarea
          name="description"
          value={form.description}
          onChange={handleChange}
          required
        />
      </div>
      <button type="submit" disabled={loading}>
        {id ? 'Update' : 'Add'} Product
      </button>
    </form>
  );
};

export default ProductForm;