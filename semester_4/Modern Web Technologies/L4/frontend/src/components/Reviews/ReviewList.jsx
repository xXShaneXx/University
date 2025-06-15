import React, { useEffect } from 'react';
import { useReviews } from '../../context/ReviewContext';
import ReviewItem from './ReviewItem';
import '../../assets/ReviewList.css';

const ReviewList = ({ productId }) => {
  const { reviews, fetchReviews, canDeleteReview } = useReviews();
  const productReviews = reviews[productId] || [];

  useEffect(() => {
    if (!productReviews.length) {
      fetchReviews(productId);
    }
  }, [productId, fetchReviews, productReviews.length]);

  if (productReviews.length === 0) {
    return (
      <div className="no-reviews">
        <p>No reviews yet. Be the first to review this product!</p>
      </div>
    );
  }

  // Calculate average rating
  const averageRating = productReviews.length > 0 
    ? (productReviews.reduce((sum, review) => sum + review.rating, 0) / productReviews.length).toFixed(1)
    : 0;

  return (
    <div className="review-list-container">
      <div className="rating-summary">
        <div className="average-rating">
          <span className="rating-number">{averageRating}</span>
          <span className="rating-out-of">/5</span>
        </div>
        <div className="rating-stars">
          {Array(5).fill().map((_, i) => (
            <span key={i} className={i < Math.round(averageRating) ? 'star-filled' : 'star-empty'}>
              ★
            </span>
          ))}
        </div>
        <div className="total-reviews">
          {productReviews.length} review{productReviews.length !== 1 ? 's' : ''}
        </div>
      </div>

      <div className="review-list">
        {productReviews.map(review => (
          <ReviewItem 
            key={review._id} 
            review={review} 
            canDelete={canDeleteReview(review)}
            productId={productId}
          />
        ))}
      </div>
    </div>
  );
};

export default ReviewList;