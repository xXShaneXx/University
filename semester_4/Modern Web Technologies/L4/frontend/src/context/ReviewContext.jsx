import React, { createContext, useState, useContext, useCallback, useMemo } from 'react';
import { AuthContext } from './AuthContext';
import { 
  addReview as addReviewApi, 
  deleteReview as deleteReviewApi,
  getReviewsByProduct 
} from '../services/reviewService';

const ReviewContext = createContext();

export const ReviewProvider = ({ children }) => {
  const [reviews, setReviews] = useState({});
  const [loading, setLoading] = useState({});
  const [errors, setErrors] = useState({});
  const { user } = useContext(AuthContext);

  const fetchReviews = useCallback(async (productId) => {
    setLoading(prev => ({ ...prev, [productId]: true }));
    setErrors(prev => ({ ...prev, [productId]: null }));
    try {
      const data = await getReviewsByProduct(productId);
      setReviews(prev => ({ 
        ...prev, 
        [productId]: data.map(review => ({
          ...review,
          user: review.user || { _id: 'unknown', username: 'Anonymous' }
        }))
      }));
    } catch (error) {
      setErrors(prev => ({ ...prev, [productId]: error.message }));
      console.error('Error fetching reviews:', error);
    } finally {
      setLoading(prev => ({ ...prev, [productId]: false }));
    }
  }, []);

  const addReview = useCallback(async (productId, reviewData) => {
    try {
      const newReview = await addReviewApi(productId, reviewData);
      setReviews(prev => ({
        ...prev,
        [productId]: [...(prev[productId] || []), newReview]
      }));
      return newReview;
    } catch (error) {
      console.error('Error adding review:', error);
      throw error;
    }
  }, []);

  const deleteReview = useCallback(async (productId, reviewId) => {
    try {
      await deleteReviewApi(reviewId);
      setReviews(prev => ({
        ...prev,
        [productId]: prev[productId].filter(r => r._id !== reviewId)
      }));
    } catch (error) {
      console.error('Error deleting review:', error);
      throw error;
    }
  }, []);

  const canDeleteReview = useCallback((review) => {
    return user && (user.role === 'admin' || user.id === review.user._id);
  }, [user]);

  const value = useMemo(() => ({
    reviews, 
    fetchReviews, 
    addReview, 
    deleteReview,
    canDeleteReview
  }), [reviews, fetchReviews, addReview, deleteReview, canDeleteReview]);

  return (
    <ReviewContext.Provider value={value}>
      {children}
    </ReviewContext.Provider>
  );
};

export const useReviews = () => useContext(ReviewContext);