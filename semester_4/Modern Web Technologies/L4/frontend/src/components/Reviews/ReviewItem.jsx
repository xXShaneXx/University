import React, { useState } from 'react';
import { useReviews } from '../../context/ReviewContext';
import '../../assets/ReviewItem.css';

const ReviewItem = ({ review, canDelete, productId }) => {
  const { deleteReview } = useReviews();
  const [isDeleting, setIsDeleting] = useState(false);

  const handleDelete = async () => {
    if (window.confirm('Are you sure you want to delete this review?')) {
      setIsDeleting(true);
      try {
        await deleteReview(productId, review._id);
      } catch (error) {
        console.error('Failed to delete review:', error);
      } finally {
        setIsDeleting(false);
      }
    }
  };

  return (
    <div className="review-item">
      <div className="review-header">
        <span className="review-author">{review.user?.username || 'Anonymous'}</span>
        <span className="review-rating">
          {Array(5).fill().map((_, i) => (
            <span key={i} className={i < review.rating ? 'star-filled' : 'star-empty'}>
              ★
            </span>
          ))}
        </span>
      </div>
      <p className="review-comment">{review.comment}</p>
      {canDelete && (
        <button 
          onClick={handleDelete} 
          disabled={isDeleting}
          className="delete-review-btn"
        >
          {isDeleting ? 'Deleting...' : 'Delete'}
        </button>
      )}
    </div>
  );
};

export default ReviewItem;