import React, { useState } from 'react';
import { useReviews } from '../../context/ReviewContext';
import '../../assets/ReviewForm.css';

const ReviewForm = ({ productId, onSuccess }) => {
  const { addReview } = useReviews();
  const [formData, setFormData] = useState({
    rating: 5,
    comment: ''
  });
  const [isSubmitting, setIsSubmitting] = useState(false);
  const [error, setError] = useState('');

  const handleChange = (e) => {
    const { name, value } = e.target;
    setFormData(prev => ({ ...prev, [name]: value }));
  };

  const handleSubmit = async (e) => {
    e.preventDefault();
    setError('');
    setIsSubmitting(true);
    
    try {
      await addReview(productId, formData);
      onSuccess?.();
    } catch (err) {
      setError(err.message || 'Failed to submit review');
    } finally {
      setIsSubmitting(false);
    }
  };

  return (
    <form onSubmit={handleSubmit} className="review-form">
      <h3>Add Your Review</h3>
      
      <div className="form-group">
        <label>Rating:</label>
        <select 
          name="rating" 
          value={formData.rating}
          onChange={handleChange}
          required
        >
          {[5, 4, 3, 2, 1].map(num => (
            <option key={num} value={num}>
              {num} Star{num !== 1 ? 's' : ''}
            </option>
          ))}
        </select>
      </div>
      
      <div className="form-group">
        <label>Comment:</label>
        <textarea
          name="comment"
          value={formData.comment}
          onChange={handleChange}
          required
          minLength="10"
          rows="4"
        />
      </div>
      
      {error && <div className="error-message">{error}</div>}
      
      <button 
        type="submit" 
        disabled={isSubmitting}
        className="submit-btn"
      >
        {isSubmitting ? 'Submitting...' : 'Submit Review'}
      </button>
    </form>
  );
};

export default ReviewForm;