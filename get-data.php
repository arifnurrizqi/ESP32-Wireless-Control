<?php

$api_key_value = "tPmAT5Ab3j7F9";

// Ambil API key dari parameter GET
$api_key = $_GET['api_key'];

// Fungsi untuk mendapatkan data katup pengguna dari tabel data_records
function get_data_from_db()
{
    include "config.php";
    
    // Lakukan koneksi ke database
    $conn = new mysqli($servername, $username, $password, $dbname);

    // Periksa koneksi
    if ($conn->connect_error) {
        die("Koneksi ke database gagal: " . $conn->connect_error);
    }

    // Buat query SQL untuk mengambil data dari tabel data_records dengan id terbaru
    $sql = "SELECT * FROM data_records ORDER BY id DESC LIMIT 1";

    // Jalankan query
    $result = $conn->query($sql);

    // Periksa apakah query berhasil dieksekusi
    if ($result && $result->num_rows > 0) {
        // Ambil data dari hasil query
        $row = $result->fetch_assoc();
        $data = array(
            'id' => $row['id'],
            'data1' => $row['data1'],
            'data2' => $row['data2'],
            'reading_time' => $row['reading_time']
        );

        // Tutup koneksi database
        $conn->close();

        return $data;
    } else {
        // Jika tidak ada data ditemukan, kembalikan null
        return null;
    }
}

if ($api_key_value == $api_key) {
    // API key valid, lanjutkan proses untuk mengambil data
    $data = get_data_from_db();

    if ($data !== null) {
        $response = array(
            'status' => 'success',
            'message' => 'Data retrieved successfully',
            'data' => $data
        );

        header('Content-Type: application/json');
        echo json_encode($response);
    } else {
        $error_response = array(
            'status' => 'error',
            'message' => 'No data found'
        );

        header('Content-Type: application/json');
        http_response_code(404); // Not Found status code
        echo json_encode($error_response);
    }
} else {
    // API key tidak valid, kirimkan respons error
    $error_response = array(
        'status' => 'error',
        'message' => 'Invalid API key'
    );

    header('Content-Type: application/json');
    http_response_code(401); // Unauthorized status code
    echo json_encode($error_response);
}
?>
