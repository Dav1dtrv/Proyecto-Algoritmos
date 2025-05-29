
document.getElementById("filtrarFecha").addEventListener("click", function () {
  const fecha = document.getElementById("buscadorFecha").value;
  const secciones = document.querySelectorAll(".historial-dia");
  secciones.forEach(sec => {
    if (fecha === "" || sec.dataset.fecha.includes(fecha)) {
      sec.style.display = "block";
    } else {
      sec.style.display = "none";
    }
  });
});
