import axios from 'axios';

const API_URL = 'http://localhost:5000/api/products';

// Set up axios instance with auth token
const api = axios.create({
  baseURL: API_URL,
});

// Add request interceptor to include auth token
api.interceptors.request.use((config) => {
  const token = localStorage.getItem('token');
  if (token) {
    config.headers.Authorization = `Bearer ${token}`;
  }
  return config;
});

export const getProducts = async (page = 1, limit = 10) => {
  try {
    const response = await api.get('/', {
      params: { page, limit }
    });
    return response.data;
  } catch (error) {
    console.error('Error fetching products:', error);
    throw error;
  }
};

export const getProductById = async (id) => {
  try {
    const response = await api.get(`/${id}`);
    return response.data;
  } catch (error) {
    console.error('Error fetching product:', error);
    throw error;
  }
};

export const searchProductsByName = async (name) => {
  try {
    const response = await api.get(`/search/${name}`);
    return response.data;
  } catch (error) {
    console.error('Error searching products:', error);
    throw error;
  }
};

export const filterProductsByPrice = async (minPrice, maxPrice, sort = 'asc') => {
  try {
    const response = await api.get('/filter/price', {
      params: { minPrice, maxPrice, sort }
    });
    return response.data;
  } catch (error) {
    console.error('Error filtering products:', error);
    throw error;
  }
};

export const addProduct = async (productData) => {
  try {
    const response = await api.post('/', productData);
    return response.data;
  } catch (error) {
    console.error('Error adding product:', error);
    throw error;
  }
};

export const updateProduct = async (id, productData) => {
  try {
    const response = await api.put(`/${id}`, productData);
    return response.data;
  } catch (error) {
    console.error('Error updating product:', error);
    throw error;
  }
};

export const deleteProduct = async (id) => {
  try {
    const response = await api.delete(`/${id}`);
    return response.data;
  } catch (error) {
    console.error('Error deleting product:', error);
    throw error;
  }
};

export const getProductReviews = async (productId) => {
  try {
    const response = await api.get(`/${productId}/reviews`);
    return response.data;
  } catch (error) {
    console.error('Error fetching product reviews:', error);
    throw error;
  }
};