

const trans = document.querySelector(".transition")
const links = document.querySelectorAll(".redirect");

links.forEach(link => {
    console.log(link.href.toString());
    if (!link.href.toString().includes("#")) {
        link.addEventListener("click", (e) => {

            e.preventDefault();
            trans.classList.add("slide");

            setTimeout(() => {
                window.location = link.href;

            }, 900);
        });
    }
    
});