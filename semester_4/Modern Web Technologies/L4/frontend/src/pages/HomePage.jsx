import React, { useContext } from 'react';
import { Link, Navigate } from 'react-router-dom';
import { AuthContext } from '../context/AuthContext';
import '../assets/HomePage.css';

const HomePage = () => {
  const { user } = useContext(AuthContext);
  
  // Redirect to dashboard if user is already logged in
  if (user) {
    return <Navigate to="/dashboard" replace />;
  }
  
  return (
    <div className="home-container">
      <h1>Welcome to Our Application</h1>
      <p>Please login or register to continue.</p>
      <div className="home-actions">
        <Link to="/login" className="home-btn">Login</Link>
        <Link to="/register" className="home-btn">Register</Link>
      </div>
    </div>
  );
};

export default HomePage;