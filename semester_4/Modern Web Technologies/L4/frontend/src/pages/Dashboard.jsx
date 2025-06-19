import React, { useContext } from 'react';
import { useAuth } from '../context/AuthContext';
import { useNavigate } from 'react-router-dom';
import '../assets/Dashboard.css';

const Dashboard = () => {
  const { user, logout } = useAuth();
  const navigate = useNavigate();

  return (
    <div className="dashboard-container">
      <div className="dashboard-header">
        <h1>Welcome, {user?.username}!</h1>
        <p>Role: {user?.role}</p>
      </div>

      <div className="dashboard-content">
        <div className="dashboard-card" onClick={() => navigate('/products')}>
          <h3>Products</h3>
          <p>Browse and manage products</p>
        </div>

        <div className="dashboard-card" onClick={() => navigate('/profile')}>
          <h3>My Profile</h3>
          <p>View and edit your profile</p>
        </div>

        {user?.role === 'admin' && (
          <div className="dashboard-card" onClick={() => navigate('/admin')}>
            <h3>Admin Panel</h3>
            <p>Access admin features</p>
          </div>
        )}

        <div className="dashboard-card" onClick={() => logout()}>
          <h3>Logout</h3>
          <p>Sign out of your account</p>
        </div>
      </div>
    </div>
  );
};

export default Dashboard;