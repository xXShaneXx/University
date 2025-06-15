import React, { useEffect } from 'react';
import { useParams } from 'react-router-dom';
import { useReviews } from '../context/ReviewContext';
import ReviewForm from '../components/Reviews/ReviewForm';
import ReviewList from '../components/Reviews/ReviewList';
import '../assets/ReviewPage.css';

const ReviewPage = () => {
  const { productId } = useParams();
  const { fetchReviews, reviews } = useReviews();

  useEffect(() => {
    fetchReviews(productId);
  }, [productId, fetchReviews]);

  const handleReviewSuccess = () => {
    fetchReviews(productId);
  };

  return (
    <div className="review-page">
      <h2>Product Reviews</h2>
      <div className="review-content">
        <div className="review-form-container">
          <ReviewForm productId={productId} onSuccess={handleReviewSuccess} />
        </div>
        <div className="review-list-container">
          <ReviewList productId={productId} />
        </div>
      </div>
    </div>
  );
};

export default ReviewPage;