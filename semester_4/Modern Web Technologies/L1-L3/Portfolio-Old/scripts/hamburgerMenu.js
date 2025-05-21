// hamburgerMenu.js
document.addEventListener("DOMContentLoaded", () => {
    const burger = document.querySelector(".burger");
    const nav = document.querySelector("nav.selection ul");
  
    burger.addEventListener("click", () => {
      nav.classList.toggle("active");
    });
  });
  