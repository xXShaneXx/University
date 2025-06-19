import React from 'react';
import { Link } from 'react-router-dom';

const NotFound = () => (
  <div style={{ textAlign: 'center', marginTop: '3rem' }}>
    <h1>404 - Page Not Found</h1>
    <p>The page you are looking for does not exist.</p>
    <Link to="/" className="home-btn">Go Home</Link>
  </div>
);

export default NotFound;