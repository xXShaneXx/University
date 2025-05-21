function loadIMG(url, id) {
    let P = new Promise(function(resolve, reject) {
        let parent = document.getElementById(id);
        let element = document.createElement("img");
        element.setAttribute("src", url);
        element.setAttribute("alt", url);
        parent.appendChild(element);
        element.onload = function() { resolve(url); };
        element.onerror = function() { reject(url); };
    });
    return P;
}

function loadIMages(urls, id) {
    let promises = urls.map((url) => loadIMG(url, id));
    Promise.all(promises)
        .then(() => console.log("All images loaded successfully"))
        .catch((err) => console.error("Error loading some images:", err));
}

document.addEventListener("DOMContentLoaded", () => {
    const wazImagesContainer = document.getElementById("waz-images");
    const urls = wazImagesContainer.getAttribute("data-urls").split(",");
    loadIMages(urls, "waz-images");
});