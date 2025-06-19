import axios from 'axios';

const API_URL = 'http://localhost:5000/api/users';

const api = axios.create({
  baseURL: API_URL,
});

api.interceptors.request.use((config) => {
  const token = localStorage.getItem('token');
  if (token) {
    config.headers.Authorization = `Bearer ${token}`;
  }
  return config;
});

export const getAllUsers = async () => {
  try {
    const response = await api.get('/all');
    return response.data;
  } catch (error) {
    throw new Error(error.response?.data?.message || 'Failed to fetch users');
  }
};

export const deleteUser = async (userId) => {
  try {
    const response = await api.delete(`/${userId}`);
    return response.data;
  } catch (error) {
    throw new Error(error.response?.data?.message || 'Failed to delete user');
  }
};

export const updateUserRole = async (userId, role) => {
  try {
    const response = await api.put(`/${userId}/role`, { role });
    return response.data;
  } catch (error) {
    throw new Error(error.response?.data?.message || 'Failed to update role');
  }
};