document.getElementById("filtrarFecha").addEventListener("click", () => {
    const buscar = prompt("Ingrese la fecha (por ejemplo: Tue May 21 2024):");
    const secciones = document.querySelectorAll(".historial-dia");
    secciones.forEach(sec => {
        const titulo = sec.querySelector("h3").textContent;
        sec.style.display = titulo.includes(buscar) ? "block" : "none";
    });
});