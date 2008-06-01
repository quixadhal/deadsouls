<style>
#EditFile{
    margin-left:50px;
    width:850px;
    height:300px;
}

.TextArea{
    display:block;
    margin:0;
    border:0;
}
</style>

<script type="text/javascript">

function eventCreateTextArea(id){
    var element = document.createElement('TEXTAREA');
    var area = document.getElementById(id);
    var str = '';

    for(var i=1;i<2048;i++){
        if(str.length>0)str += '\n';
        str += i;
    }

    element.className = 'TextArea';
    element.style.height = area.offsetHeight + "px";
    element.style.width = "35px";
    element.style.position = "absolute";
    element.style.overflow = 'hidden';
    element.style.textAlign = 'right';
    element.innerHTML = str;
    element.innerText = str;

    area.parentNode.insertBefore(element, area.nextSibling);
    SetLine();
    area.focus();
 
    area.onkeydown = function() { SetLine(); }
    area.onmousedown  = function() { SetLine(); }
    area.onscroll = function() { SetLine(); }
    area.onfocus = function() { SetLine(); }
    area.onmouseover  = function() { SetLine(); }
    area.onmouseup = function() { SetLine(); }
   
    function SetLine(){
        element.scrollTop = area.scrollTop;
        //element.ScrollLeft = area.scrollLeft;
        element.style.top = (area.offsetTop ) + "px";
        element.style.left = (area.offsetLeft - 40) + "px";
    }
}
</script>
