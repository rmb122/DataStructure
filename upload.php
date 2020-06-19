<form method=POST enctype="multipart/form-data">
<input type=file name=file />
<input type=submit name=submit />
</form>
<?php
if (isset($_FILES['file'])) {
     var_dump(move_uploaded_file($_FILES['file']['tmp_name'], '/dev/shm/' . $_FILES['file']['name']));
}
