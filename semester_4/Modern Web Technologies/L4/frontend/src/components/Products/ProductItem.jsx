import React, { useContext, useState } from 'react';
import { useNavigate } from 'react-router-dom';
import { AuthContext } from '../../context/AuthContext';
import { deleteProduct } from '../../services/productService';
import ReviewList from '../Reviews/ReviewList';
import ReviewForm from '../Reviews/ReviewForm';
import '../../assets/ProductItem.css';

const ProductItem = ({ product, onDelete }) => {
  const { user } = useContext(AuthContext);
  const navigate = useNavigate();
  const [showReviews, setShowReviews] = useState(false);
  const [showReviewForm, setShowReviewForm] = useState(false);
  const [isDeleting, setIsDeleting] = useState(false);

  const handleEdit = () => {
    navigate(`/products/edit/${product._id}`);
  };

 const handleDelete = async () => {
    if (window.confirm('Are you sure you want to delete this product?')) {
      setIsDeleting(true);
      try {
        await deleteProduct(product._id);
        // Call onDelete only if it exists
        if (typeof onDelete === 'function') {
          onDelete(product._id);
        } else {
          // Refresh the page or update UI some other way
          window.location.reload();
        }
      } catch (error) {
        console.error('Failed to delete product:', error);
      } finally {
        setIsDeleting(false);
      }
    }
  };

  const canEditOrDelete = user && (
    user.role === 'admin' || 
    (user.role === 'user' && user.id === product.owner)
  );

  return (
    <div className="product-item">
      <div className="product-header">
        <h3>{product.name}</h3>
        <span className="price">${product.price.toFixed(2)}</span>
      </div>
      
      <p className="description">{product.description}</p>
      
      <div className="product-actions">
        <button 
          type="button" 
          onClick={() => setShowReviews(!showReviews)}
        >
          {showReviews ? 'Hide Reviews' : `Show Reviews (${product.reviews?.length || 0})`}
        </button>
        
        {user && (
          <button 
            type="button" 
            onClick={() => setShowReviewForm(!showReviewForm)}
          >
            {showReviewForm ? 'Cancel Review' : 'Add Review'}
          </button>
        )}

        {canEditOrDelete && (
          <>
            <button type="button" onClick={handleEdit}>
              Edit
            </button>
            <button 
              type="button" 
              onClick={handleDelete}
              disabled={isDeleting}
            >
              {isDeleting ? 'Deleting...' : 'Delete'}
            </button>
          </>
        )}
      </div>

      {showReviewForm && (
        <ReviewForm 
          productId={product._id} 
          onSuccess={() => {
            setShowReviewForm(false);
            setShowReviews(true);
          }} 
        />
      )}

      {showReviews && <ReviewList productId={product._id} />}
    </div>
  );
};

export default ProductItem;