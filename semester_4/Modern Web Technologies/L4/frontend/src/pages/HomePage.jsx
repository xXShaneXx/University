import React from 'react';
import { Link } from 'react-router-dom';
import '../assets/HomePage.css';

const HomePage = () => {
  return (
    <div className="home-container">
      <h1>Welcome to Our Application</h1>
      <p>Please login or register to continue.</p>
      <div className="home-actions">
        <Link to="/login" className="home-btn">Login</Link>
        <Link to="/register-user" className="home-btn">Register</Link>
      </div>
    </div>
  );
};

export default HomePage;