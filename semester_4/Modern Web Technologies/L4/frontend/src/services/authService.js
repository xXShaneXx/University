import axios from 'axios';

const API_URL = 'http://localhost:5000/api/auth';

export const loginUser = async (credentials) => {
  try {
    const response = await axios.post(`${API_URL}/login`, credentials);
    return response.data;
  } catch (error) {
    if (error.response) {
      throw new Error(error.response.data.message || 'Login failed');
    } else {
      throw new Error('Network error. Please try again.');
    }
  }
};

export const registerUser = async (userData) => {
  try {
    const response = await axios.post(`${API_URL}/register-user`, userData);
    return response.data;
  } catch (error) {
    if (error.response) {
      throw new Error(
        error.response.data.message || 
        error.response.data.error || 
        'Registration failed'
      );
    } else {
      throw new Error('Network error. Please try again.');
    }
  }
};

export const registerAdmin = async (userData, token) => {
  try {
    console.log('Registering admin with token:', token);
    console.log('User data:', userData);
    
    const response = await axios.post(`${API_URL}/register-admin`, userData, {
      headers: { Authorization: `Bearer ${token}` }
    });
    return response.data;
  } catch (error) {
    console.error('Admin registration error:', error.response?.data || error.message);
    if (error.response) {
      throw new Error(error.response.data.message || 'Admin registration failed');
    } else {
      throw new Error('Network error. Please try again.');
    }
  }
};