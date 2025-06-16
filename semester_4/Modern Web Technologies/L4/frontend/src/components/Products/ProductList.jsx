import React, { useContext, useEffect, useState } from 'react';
import { useNavigate } from 'react-router-dom';
import { AuthContext } from '../../context/AuthContext';
import { getProducts, filterProductsByPrice } from '../../services/productService';
import ProductItem from './ProductItem';
import '../../assets/ProductList.css';

const ProductList = () => {
  const [products, setProducts] = useState([]);
  const [loading, setLoading] = useState(true);
  const { user } = useContext(AuthContext);
  const navigate = useNavigate();
  
  // State for filters
  const [minPrice, setMinPrice] = useState('');
  const [maxPrice, setMaxPrice] = useState('');
  const [sortOrder, setSortOrder] = useState('asc');
  const [showFilters, setShowFilters] = useState(false);

  const fetchProducts = async () => {
    setLoading(true);
    try {
      const data = await getProducts();
      setProducts(Array.isArray(data) ? data : data.products || []);
    } catch (err) {
      console.error(err);
    } finally {
      setLoading(false);
    }
  };

  const applyFilters = async () => {
    setLoading(true);
    try {
      const filteredProducts = await filterProductsByPrice(
        minPrice || undefined,
        maxPrice || undefined,
        sortOrder
      );
      setProducts(Array.isArray(filteredProducts) ? filteredProducts : []);
    } catch (err) {
      console.error(err);
    } finally {
      setLoading(false);
    }
  };

  const resetFilters = () => {
    setMinPrice('');
    setMaxPrice('');
    setSortOrder('asc');
    fetchProducts();
  };

  useEffect(() => {
    fetchProducts();
  }, []);

  if (loading) return <div className="loading">Loading...</div>;

  return (
    <div className="product-list">
      <div className="product-list-header">
        <h2>Products</h2>
        <div className="product-list-actions">
          <button 
            className="filter-toggle-btn"
            onClick={() => setShowFilters(!showFilters)}
          >
            {showFilters ? 'Hide Filters' : 'Show Filters'}
          </button>
          
          {(user?.role === 'admin' || user?.role === 'user') && (
            <button className="add-product-btn" onClick={() => navigate('/products/add')}>
              Add Product
            </button>
          )}
        </div>
      </div>

      {showFilters && (
        <div className="filter-section">
          <div className="filter-controls">
            <div className="filter-group">
              <label>Min Price:</label>
              <input
                type="number"
                value={minPrice}
                onChange={(e) => setMinPrice(e.target.value)}
                min="0"
                step="0.01"
              />
            </div>
            
            <div className="filter-group">
              <label>Max Price:</label>
              <input
                type="number"
                value={maxPrice}
                onChange={(e) => setMaxPrice(e.target.value)}
                min="0"
                step="0.01"
              />
            </div>
            
            <div className="filter-group">
              <label>Sort:</label>
              <select
                value={sortOrder}
                onChange={(e) => setSortOrder(e.target.value)}
              >
                <option value="asc">Price: Low to High</option>
                <option value="desc">Price: High to Low</option>
              </select>
            </div>
            
            <div className="filter-buttons">
              <button onClick={applyFilters}>Apply Filters</button>
              <button onClick={resetFilters}>Reset</button>
            </div>
          </div>
        </div>
      )}

      {products.length === 0 ? (
        <div className="no-products">No products found matching your filters.</div>
      ) : (
        <div className="products-grid">
          {products.map((product) => (
            <ProductItem key={product._id} product={product} />
          ))}
        </div>
      )}
    </div>
  );
};

export default ProductList;