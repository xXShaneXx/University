import React, { useState, useEffect, useContext } from 'react';
import { useAuth } from '../context/AuthContext';
import { getAllUsers, deleteUser, updateUserRole } from '../services/userService';
import { registerAdmin } from '../services/authService';
import '../assets/AdminPanel.css';

const AdminPanel = () => {
  const { user } = useAuth();
  const [users, setUsers] = useState([]);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState('');
  const [newAdminData, setNewAdminData] = useState({
    username: '',
    password: '',
    email: ''
  });

  useEffect(() => {
    const fetchUsers = async () => {
      try {
        const data = await getAllUsers();
        setUsers(data.users);
      } catch (err) {
        setError(err.message || 'Failed to fetch users');
      } finally {
        setLoading(false);
      }
    };
    fetchUsers();
  }, []);

  const handleDeleteUser = async (userId) => {
    if (window.confirm('Are you sure you want to delete this user?')) {
      try {
        await deleteUser(userId);
        setUsers(users.filter(u => u._id !== userId));
      } catch (err) {
        setError(err.message || 'Failed to delete user');
      }
    }
  };

  const handleRoleChange = async (userId, newRole) => {
    try {
      await updateUserRole(userId, newRole);
      setUsers(users.map(u => 
        u._id === userId ? { ...u, role: newRole } : u
      ));
    } catch (err) {
      setError(err.message || 'Failed to update role');
    }
  };

  const handleAdminRegister = async (e) => {
    e.preventDefault();
    try {
      await registerAdmin(newAdminData, user.token);
      setNewAdminData({ username: '', password: '', email: '' });
      setError('');
      alert('New admin registered successfully!');
    } catch (err) {
      setError(err.message || 'Failed to register admin');
    }
  };

  if (loading) return <div className="loading">Loading...</div>;

  return (
    <div className="admin-container">
      <h1>Admin Panel</h1>
      
      {error && <div className="error-message">{error}</div>}

      <div className="admin-sections">
        <section className="user-management">
          <h2>User Management</h2>
          <div className="user-list">
            {users.map(userItem => (
              <div key={userItem._id} className="user-card">
                <div className="user-info">
                  <span className="username">{userItem.username}</span>
                  <span className="email">{userItem.email}</span>
                  <span className={`role ${userItem.role}`}>{userItem.role}</span>
                </div>
                <div className="user-actions">
                  <select
                    value={userItem.role}
                    onChange={(e) => handleRoleChange(userItem._id, e.target.value)}
                    disabled={userItem._id === user.id}
                  >
                    <option value="user">User</option>
                    <option value="admin">Admin</option>
                  </select>
                  <button
                    onClick={() => handleDeleteUser(userItem._id)}
                    disabled={userItem._id === user.id}
                    className="delete-btn"
                  >
                    Delete
                  </button>
                </div>
              </div>
            ))}
          </div>
        </section>

        <section className="admin-registration">
          <h2>Register New Admin</h2>
          <form onSubmit={handleAdminRegister}>
            <div className="form-group">
              <label>Username</label>
              <input
                type="text"
                value={newAdminData.username}
                onChange={(e) => setNewAdminData({...newAdminData, username: e.target.value})}
                required
                minLength="3"
              />
            </div>
            <div className="form-group">
              <label>Email</label>
              <input
                type="email"
                value={newAdminData.email}
                onChange={(e) => setNewAdminData({...newAdminData, email: e.target.value})}
                required
              />
            </div>
            <div className="form-group">
              <label>Password</label>
              <input
                type="password"
                value={newAdminData.password}
                onChange={(e) => setNewAdminData({...newAdminData, password: e.target.value})}
                required
                minLength="6"
              />
            </div>
            <button type="submit" className="register-btn">
              Register Admin
            </button>
          </form>
        </section>
      </div>
    </div>
  );
};

export default AdminPanel;