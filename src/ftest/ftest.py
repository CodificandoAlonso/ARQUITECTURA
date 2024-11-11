import unittest
import random
import subprocess

version = "debug"
exe_imtoolaos = f"../cmake-build-{version}/imtool-aos/imtool-aos"
exe_imtoolsoa = f"../cmake-build-{version}/imtool-soa/imtool-soa"

def generate_ppm_f3(width, height, filename):
    with open(filename, 'wb') as f:
        f.write(f'P6\n{width} {height}\n255\n'.encode())
        for _ in range(width * height):
            f.write(bytes([random.randint(0, 255), random.randint(0, 255), random.randint(0, 255)]))

def read_ppm_header(file):
    """Reads the header of a PPM file and returns it as a tuple."""
    header = []
    while len(header) < 4:  # Queremos 4 elementos: "P6", ancho, alto, y max color
        line = file.readline().decode().strip()
        header.extend(line.split())
    return header[0], int(header[1]), int(header[2]), int(header[3])

class TestImtoolAOS(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        """Generates test images only once for all tests."""
        cls.img1_f3 = "img1.ppm"
        cls.img1_f3_width = 100
        cls.img1_f3_height = 100
        cls.img2_f3 = "img2.ppm"
        cls.img2_f3_width = 100
        cls.img2_f3_height = 100
        generate_ppm_f3(cls.img1_f3_width, cls.img1_f3_height, cls.img1_f3)
        generate_ppm_f3(cls.img2_f3_width, cls.img2_f3_height, cls.img2_f3)

    def check_images(self, img1, img2):
        with open(img1, 'rb') as f1, open(img2, 'rb') as f2:
            # Leemos la cabezera de ambos archivos, sabiendo que los elementos pueden estar separados
            # por espacios o saltos de línea.
            header1 = read_ppm_header(f1)
            header2 = read_ppm_header(f2)
            self.assertEqual(header1, header2)

            # Comparamos el contenido de ambos archivos.
            self.assertEqual(f1.read(), f2.read())

    def test_info_ok1(self):
        result = subprocess.run(f'{exe_imtoolaos} {self.img1_f3} {self.img2_f3} info', shell=True, capture_output=True)
        self.assertEqual(result.returncode, 0)
        self.assertTrue("Format: P6" in result.stdout.decode())
        self.assertTrue(f"Width: {self.img1_f3_width}" in result.stdout.decode())
        self.assertTrue(f"Height: {self.img1_f3_height}" in result.stdout.decode())
        self.assertTrue("Max color value: 255" in result.stdout.decode())

    def test_info_ok2(self):
        result = subprocess.run(f'{exe_imtoolaos} {self.img2_f3} {self.img1_f3} info', shell=True, capture_output=True)
        self.assertEqual(result.returncode, 0)
        self.assertTrue("Format: P6" in result.stdout.decode())
        self.assertTrue(f"Width: {self.img2_f3_width}" in result.stdout.decode())
        self.assertTrue(f"Height: {self.img2_f3_height}" in result.stdout.decode())
        self.assertTrue("Max color value: 255" in result.stdout.decode())

    def test_info_nok1(self):
        result = subprocess.run(f'{exe_imtoolaos} {self.img1_f3} {self.img2_f3} info info', shell=True, capture_output=True)
        self.assertNotEqual(result.returncode, 0)
        self.assertTrue("Invalid number of arguments for option info: 5" in result.stderr.decode())

    def test_info_nok2(self):
        result = subprocess.run(f'{exe_imtoolaos} {self.img1_f3} {self.img2_f3} info info info', shell=True, capture_output=True)
        self.assertNotEqual(result.returncode, 0)
        self.assertTrue("Invalid number of arguments for option info: 6" in result.stderr.decode())

    def test_maxlevel_ok1(self):
        result_aos = subprocess.run(f'{exe_imtoolaos} ./input/deer-small.ppm out_aos.ppm maxlevel 100', shell=True, capture_output=True)
        result_soa = subprocess.run(f'{exe_imtoolsoa} ./input/deer-small.ppm out_soa.ppm maxlevel 100', shell=True, capture_output=True)
        self.assertEqual(result_aos.returncode, 0)
        self.assertEqual(result_soa.returncode, 0)
        self.check_images("out_aos.ppm", "./expected/maxlevel/deer-small-100.ppm")
        self.check_images("out_soa.ppm", "./expected/maxlevel/deer-small-100.ppm")

    def test_maxlevel_ok2(self):
        result_aos = subprocess.run(f'{exe_imtoolaos} ./input/deer-small.ppm out_aos.ppm maxlevel 255', shell=True, capture_output=True)
        result_soa = subprocess.run(f'{exe_imtoolsoa} ./input/deer-small.ppm out_soa.ppm maxlevel 255', shell=True, capture_output=True)
        self.assertEqual(result_aos.returncode, 0)
        self.assertEqual(result_soa.returncode, 0)
        self.check_images("out_aos.ppm", "./expected/maxlevel/deer-small-255.ppm")
        self.check_images("out_soa.ppm", "./expected/maxlevel/deer-small-255.ppm")

    def test_maxlevel_ok3(self):
        result_aos = subprocess.run(f'{exe_imtoolaos} ./input/deer-small.ppm out_aos.ppm maxlevel 65535', shell=True, capture_output=True)
        result_soa = subprocess.run(f'{exe_imtoolsoa} ./input/deer-small.ppm out_soa.ppm maxlevel 65535', shell=True, capture_output=True)
        self.assertEqual(result_aos.returncode, 0)
        self.assertEqual(result_soa.returncode, 0)
        self.check_images("out_soa.ppm", "./expected/maxlevel/deer-small-65535.ppm")
        self.check_images("out_aos.ppm", "./expected/maxlevel/deer-small-65535.ppm")

    def test_maxlevel_ok4(self):
        result_aos = subprocess.run(f'{exe_imtoolaos} ./input/lake-small.ppm out_aos.ppm maxlevel 255', shell=True, capture_output=True)
        result_soa = subprocess.run(f'{exe_imtoolsoa} ./input/lake-small.ppm out_soa.ppm maxlevel 255', shell=True, capture_output=True)
        self.assertEqual(result_aos.returncode, 0)
        self.assertEqual(result_soa.returncode, 0)
        self.check_images("out_aos.ppm", "./expected/maxlevel/lake-small-255.ppm")
        self.check_images("out_soa.ppm", "./expected/maxlevel/lake-small-255.ppm")

    def test_maxlevel_ok5(self):
        result_aos = subprocess.run(f'{exe_imtoolaos} ./input/lake-small.ppm out_aos.ppm maxlevel 65535', shell=True, capture_output=True)
        result_soa = subprocess.run(f'{exe_imtoolsoa} ./input/lake-small.ppm out_soa.ppm maxlevel 65535', shell=True, capture_output=True)
        self.assertEqual(result_aos.returncode, 0)
        self.assertEqual(result_soa.returncode, 0)
        self.check_images("out_aos.ppm", "./expected/maxlevel/lake-small-65535.ppm")
        self.check_images("out_soa.ppm", "./expected/maxlevel/lake-small-65535.ppm")

    def test_maxlevel_nok1(self):
        result_aos = subprocess.run(f'{exe_imtoolaos} ./input/lake-small.ppm out_aos.ppm maxlevel', shell=True, capture_output=True)
        result_soa = subprocess.run(f'{exe_imtoolsoa} ./input/lake-small.ppm out_soa.ppm maxlevel', shell=True, capture_output=True)
        self.assertNotEqual(result_aos.returncode, 0)
        self.assertNotEqual(result_soa.returncode, 0)
        self.assertTrue("Invalid number of arguments for option maxlevel: 4" in result_aos.stderr.decode())
        self.assertTrue("Invalid number of arguments for option maxlevel: 4" in result_soa.stderr.decode())

    def test_maxlevel_nok2(self):
        result_aos = subprocess.run(f'{exe_imtoolaos} ./input/lake-small.ppm out_aos.ppm maxlevel 100 100', shell=True, capture_output=True)
        result_soa = subprocess.run(f'{exe_imtoolsoa} ./input/lake-small.ppm out_soa.ppm maxlevel 100 100', shell=True, capture_output=True)
        self.assertNotEqual(result_aos.returncode, 0)
        self.assertNotEqual(result_soa.returncode, 0)
        self.assertTrue("Invalid number of arguments for option maxlevel: 6" in result_aos.stderr.decode())
        self.assertTrue("Invalid number of arguments for option maxlevel: 6" in result_soa.stderr.decode())

    def test_maxlevel_nok3(self):
        result_aos = subprocess.run(f'{exe_imtoolaos} ./input/lake-small.ppm out_aos.ppm maxlevel -100', shell=True, capture_output=True)
        result_soa = subprocess.run(f'{exe_imtoolsoa} ./input/lake-small.ppm out_soa.ppm maxlevel -100', shell=True, capture_output=True)
        self.assertNotEqual(result_aos.returncode, 0)
        self.assertNotEqual(result_soa.returncode, 0)
        self.assertTrue("Error: Invalid argument for option maxlevel: -100" in result_aos.stderr.decode())
        self.assertTrue("Error: Invalid argument for option maxlevel: -100" in result_soa.stderr.decode())

    def test_maxlevel_nok4(self):
        result_aos = subprocess.run(f'{exe_imtoolaos} ./input/lake-small.ppm out_aos.ppm maxlevel foo', shell=True, capture_output=True)
        result_soa = subprocess.run(f'{exe_imtoolsoa} ./input/lake-small.ppm out_soa.ppm maxlevel foo', shell=True, capture_output=True)
        self.assertNotEqual(result_aos.returncode, 0)
        self.assertNotEqual(result_soa.returncode, 0)
        self.assertTrue("Error: Invalid argument for option maxlevel: foo" in result_aos.stderr.decode())
        self.assertTrue("Error: Invalid argument for option maxlevel: foo" in result_soa.stderr.decode())

    def test_maxlevel_nok5(self):
        result_aos = subprocess.run(f'{exe_imtoolaos} vacio out_aos.ppm maxlevel 100', shell=True, capture_output=True)
        result_soa = subprocess.run(f'{exe_imtoolsoa} vacio out_soa.ppm maxlevel 100', shell=True, capture_output=True)
        self.assertTrue("Error al abrir el archivo de entrada" in result_aos.stderr.decode())
        self.assertTrue("Error al abrir el archivo de entrada" in result_soa.stderr.decode())


    def test_resize_ok1(self):
        result_aos = subprocess.run(f'{exe_imtoolaos} ./input/deer-large.ppm out_aos.ppm resize 100 100', shell=True, capture_output=True)
        result_soa = subprocess.run(f'{exe_imtoolsoa} ./input/deer-large.ppm out_soa.ppm resize 100 100', shell=True, capture_output=True)
        self.assertEqual(result_aos.returncode, 0)
        self.assertEqual(result_soa.returncode, 0)
        self.check_images("out_aos.ppm", "./expected/resize/deer-large-100.ppm")
        self.check_images("out_soa.ppm", "./expected/resize/deer-large-100.ppm")

    def test_resize_ok2(self):
        result_aos = subprocess.run(f'{exe_imtoolaos} ./input/deer-large.ppm out_aos.ppm resize 1000 1000', shell=True, capture_output=True)
        result_soa = subprocess.run(f'{exe_imtoolsoa} ./input/deer-large.ppm out_soa.ppm resize 1000 1000', shell=True, capture_output=True)
        self.assertEqual(result_aos.returncode, 0)
        self.assertEqual(result_soa.returncode, 0)
        self.check_images("out_aos.ppm", "./expected/resize/deer-large-1000.ppm")
        self.check_images("out_soa.ppm", "./expected/resize/deer-large-1000.ppm")

    def test_resize_ok3(self):
        result_aos = subprocess.run(f'{exe_imtoolaos} ./input/deer-small.ppm out_aos.ppm resize 100 100', shell=True, capture_output=True)
        result_soa = subprocess.run(f'{exe_imtoolsoa} ./input/deer-small.ppm out_soa.ppm resize 100 100', shell=True, capture_output=True)
        self.assertEqual(result_aos.returncode, 0)
        self.assertEqual(result_soa.returncode, 0)
        self.check_images("out_aos.ppm", "./expected/resize/deer-small-100.ppm")
        self.check_images("out_soa.ppm", "./expected/resize/deer-small-100.ppm")

    def test_resize_ok4(self):
        result_aos = subprocess.run(f'{exe_imtoolaos} ./input/deer-small.ppm out_aos.ppm resize 1000 1000', shell=True, capture_output=True)
        result_soa = subprocess.run(f'{exe_imtoolsoa} ./input/deer-small.ppm out_soa.ppm resize 1000 1000', shell=True, capture_output=True)
        self.assertEqual(result_aos.returncode, 0)
        self.assertEqual(result_soa.returncode, 0)
        self.check_images("out_aos.ppm", "./expected/resize/deer-small-1000.ppm")
        self.check_images("out_soa.ppm", "./expected/resize/deer-small-1000.ppm")

    def test_resize_nok1(self):
        result_aos = subprocess.run(f'{exe_imtoolaos} ./input/deer-small.ppm out_aos.ppm resize', shell=True, capture_output=True)
        result_soa = subprocess.run(f'{exe_imtoolsoa} ./input/deer-small.ppm out_soa.ppm resize', shell=True, capture_output=True)
        self.assertNotEqual(result_aos.returncode, 0)
        self.assertNotEqual(result_soa.returncode, 0)
        self.assertTrue("Invalid number of arguments for option resize: 4" in result_aos.stderr.decode())
        self.assertTrue("Invalid number of arguments for option resize: 4" in result_soa.stderr.decode())

    def test_resize_nok2(self):
        result_aos = subprocess.run(f'{exe_imtoolaos} ./input/deer-small.ppm out_aos.ppm resize 100', shell=True, capture_output=True)
        result_soa = subprocess.run(f'{exe_imtoolsoa} ./input/deer-small.ppm out_soa.ppm resize 100', shell=True, capture_output=True)
        self.assertNotEqual(result_aos.returncode, 0)
        self.assertNotEqual(result_soa.returncode, 0)
        self.assertTrue("Invalid number of arguments for option resize: 5" in result_aos.stderr.decode())
        self.assertTrue("Invalid number of arguments for option resize: 5" in result_soa.stderr.decode())

    def test_resize_nok3(self):
        result_aos = subprocess.run(f'{exe_imtoolaos} ./input/deer-small.ppm out_aos.ppm resize 100 100 100', shell=True, capture_output=True)
        result_soa = subprocess.run(f'{exe_imtoolsoa} ./input/deer-small.ppm out_soa.ppm resize 100 100 100', shell=True, capture_output=True)
        self.assertNotEqual(result_aos.returncode, 0)
        self.assertNotEqual(result_soa.returncode, 0)
        self.assertTrue("Invalid number of arguments for option resize: 7" in result_aos.stderr.decode())
        self.assertTrue("Invalid number of arguments for option resize: 7" in result_soa.stderr.decode())

    def test_resize_nok4(self):
        result_aos = subprocess.run(f'{exe_imtoolaos} ./input/deer-small.ppm out_aos.ppm resize 100 foo', shell=True, capture_output=True)
        result_soa = subprocess.run(f'{exe_imtoolsoa} ./input/deer-small.ppm out_soa.ppm resize 100 foo', shell=True, capture_output=True)
        self.assertNotEqual(result_aos.returncode, 0)
        self.assertNotEqual(result_soa.returncode, 0)
        self.assertTrue("Error: Invalid resize height: foo" in result_aos.stderr.decode())
        self.assertTrue("Error: Invalid resize height: foo" in result_soa.stderr.decode())

    def test_resize_nok5(self):
        result_aos = subprocess.run(f'{exe_imtoolaos} ./input/deer-small.ppm out_aos.ppm resize foo 100', shell=True, capture_output=True)
        result_soa = subprocess.run(f'{exe_imtoolsoa} ./input/deer-small.ppm out_soa.ppm resize foo 100', shell=True, capture_output=True)
        self.assertNotEqual(result_aos.returncode, 0)
        self.assertNotEqual(result_soa.returncode, 0)
        self.assertTrue("Error: Invalid resize width: foo" in result_aos.stderr.decode())
        self.assertTrue("Error: Invalid resize width: foo" in result_soa.stderr.decode())

    def test_resize_nok6(self):
        result_aos = subprocess.run(f'{exe_imtoolaos} vacio out_aos.ppm resize 100 100', shell=True, capture_output=True)
        result_soa = subprocess.run(f'{exe_imtoolsoa} vacio out_soa.ppm resize 100 100', shell=True, capture_output=True)
        self.assertTrue("Error al abrir el archivo de entrada" in result_aos.stderr.decode())
        self.assertTrue("Error al abrir el archivo de entrada" in result_soa.stderr.decode())

    def test_compress_ok1(self):
        result_aos = subprocess.run(f'{exe_imtoolaos} ./input/deer-small.ppm out_aos.cppm compress', shell=True, capture_output=True)
        result_soa = subprocess.run(f'{exe_imtoolsoa} ./input/deer-small.ppm out_soa.cppm compress', shell=True, capture_output=True)
        self.assertEqual(result_aos.returncode, 0)
        self.assertEqual(result_soa.returncode, 0)

        # recuperamos las imágenes comprimidas y la original
        subprocess.run(f'./recover out_aos.cppm', shell=True)
        subprocess.run(f'./recover out_soa.cppm', shell=True)
        subprocess.run(f'./recover ./expected/compress/deer-small.cppm', shell=True)

        # comparamos las imágenes recuperadas con la original
        self.check_images("out_aos.ppm", "./input/deer-small.ppm")
        self.check_images("out_soa.ppm", "./input/deer-small.ppm")




if __name__ == "__main__":
    # Compilamos la herramienta recover
    subprocess.run("g++ recover.cpp -o recover", shell=True)
    unittest.main()
    subprocess.run("rm *.ppm", shell=True)
    subprocess.run("rm *.cppm", shell=True)