import unittest
import random
import subprocess

version = "release"
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

    def check_images(self, img1, img2, trheadshold=100.0):
        with open(img1, 'rb') as f1, open(img2, 'rb') as f2:
            header1 = read_ppm_header(f1)
            header2 = read_ppm_header(f2)
            self.assertEqual(header1, header2)

            # Comparamos el contenido de ambos archivos.
            data1 = f1.read()
            data2 = f2.read()
            equal = 0
            not_equal = 0
            for i, (byte1, byte2) in enumerate(zip(data1, data2)):
                # Permitiremos un nivel de error de +-1 en los bytes.
                # ya que las imágenes de salida esperada algunas veces redondean los valores
                # y nosotros los truncamos.
                if abs(byte1 - byte2) <= 1:
                    equal += 1
                else:
                    not_equal += 1

            # Calculamos el ratio de aciertos, que obligamos a que sea siempre un 100% de acierto
            # excepto en CUTFREQ, donde permitimos un 99.5% de acierto. (ya que la naturaleza de la
            # función no garantiza que el resultado sea exactamente igual).
            total = equal + not_equal
            ratio = (equal * 100) / total
            self.assertTrue(ratio >= trheadshold, f"Ratio: {ratio}")


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
        result_aos = subprocess.run(f'{exe_imtoolaos} ./input/lake-large.ppm out_aos.ppm resize 100 100', shell=True, capture_output=True)
        result_soa = subprocess.run(f'{exe_imtoolsoa} ./input/lake-large.ppm out_soa.ppm resize 100 100', shell=True, capture_output=True)
        self.assertEqual(result_aos.returncode, 0)
        self.assertEqual(result_soa.returncode, 0)
        self.check_images("out_aos.ppm", "./expected/resize/lake-large-100.ppm")
        self.check_images("out_soa.ppm", "./expected/resize/lake-large-100.ppm")

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

    def test_cutfreq_ok1(self):
        result_aos = subprocess.run(f'{exe_imtoolaos} ./input/lake-large.ppm out_aos.ppm cutfreq 100000', shell=True, capture_output=True)
        result_soa = subprocess.run(f'{exe_imtoolsoa} ./input/lake-large.ppm out_soa.ppm cutfreq 100000', shell=True, capture_output=True)
        self.assertEqual(result_aos.returncode, 0)
        self.assertEqual(result_soa.returncode, 0)
        self.check_images("out_aos.ppm", "./expected/cutfreq/lake-large-100K.ppm", 99.5)
        self.check_images("out_soa.ppm", "./expected/cutfreq/lake-large-100K.ppm", 99.5)

    def test_cutfreq_ok2(self):
        result_aos = subprocess.run(f'{exe_imtoolaos} ./input/lake-large.ppm out_aos.ppm cutfreq 162000', shell=True, capture_output=True)
        result_soa = subprocess.run(f'{exe_imtoolsoa} ./input/lake-large.ppm out_soa.ppm cutfreq 162000', shell=True, capture_output=True)
        self.assertEqual(result_aos.returncode, 0)
        self.assertEqual(result_soa.returncode, 0)
        self.check_images("out_aos.ppm", "./expected/cutfreq/lake-large-162K.ppm", 97)
        self.check_images("out_soa.ppm", "./expected/cutfreq/lake-large-162K.ppm", 97)

    def test_cutfreq_nok1(self):
        result_aos = subprocess.run(f'{exe_imtoolaos} ./input/lake-large.ppm out_aos.ppm cutfreq', shell=True, capture_output=True)
        result_soa = subprocess.run(f'{exe_imtoolsoa} ./input/lake-large.ppm out_soa.ppm cutfreq', shell=True, capture_output=True)
        self.assertNotEqual(result_aos.returncode, 0)
        self.assertNotEqual(result_soa.returncode, 0)
        self.assertTrue("Invalid number of arguments for cutfreq: 4" in result_aos.stderr.decode())
        self.assertTrue("Invalid number of arguments for cutfreq: 4" in result_soa.stderr.decode())

    def test_cutfreq_nok2(self):
        result_aos = subprocess.run(f'{exe_imtoolaos} ./input/lake-large.ppm out_aos.ppm cutfreq 100000 100000', shell=True, capture_output=True)
        result_soa = subprocess.run(f'{exe_imtoolsoa} ./input/lake-large.ppm out_soa.ppm cutfreq 100000 100000', shell=True, capture_output=True)
        self.assertNotEqual(result_aos.returncode, 0)
        self.assertNotEqual(result_soa.returncode, 0)
        self.assertTrue("Invalid number of arguments for cutfreq: 6" in result_aos.stderr.decode())
        self.assertTrue("Invalid number of arguments for cutfreq: 6" in result_soa.stderr.decode())

    def test_cutfreq_nok3(self):
        result_aos = subprocess.run(f'{exe_imtoolaos} ./input/lake-large.ppm out_aos.ppm cutfreq foo', shell=True, capture_output=True)
        result_soa = subprocess.run(f'{exe_imtoolsoa} ./input/lake-large.ppm out_soa.ppm cutfreq foo', shell=True, capture_output=True)
        self.assertNotEqual(result_aos.returncode, 0)
        self.assertNotEqual(result_soa.returncode, 0)
        self.assertTrue("Error: Invalid cutfreq: foo" in result_aos.stderr.decode())
        self.assertTrue("Error: Invalid cutfreq: foo" in result_soa.stderr.decode())

    def test_cutfreq_nok4(self):
        result_aos = subprocess.run(f'{exe_imtoolaos} ./input/lake-large.ppm out_aos.ppm cutfreq -100', shell=True, capture_output=True)
        result_soa = subprocess.run(f'{exe_imtoolsoa} ./input/lake-large.ppm out_soa.ppm cutfreq -100', shell=True, capture_output=True)
        self.assertNotEqual(result_aos.returncode, 0)
        self.assertNotEqual(result_soa.returncode, 0)
        self.assertTrue("Error: Invalid cutfreq: -100" in result_aos.stderr.decode())
        self.assertTrue("Error: Invalid cutfreq: -100" in result_soa.stderr.decode())

    def test_cutfreq_nok5(self):
        result_aos = subprocess.run(f'{exe_imtoolaos} ./input/sabatini.ppm out_aos.ppm cutfreq 100000000', shell=True, capture_output=True)
        result_soa = subprocess.run(f'{exe_imtoolsoa} ./input/sabatini.ppm out_soa.ppm cutfreq 100000000', shell=True, capture_output=True)
        self.assertNotEqual(result_aos.returncode, 0)
        self.assertNotEqual(result_soa.returncode, 0)
        self.assertTrue("El numero de pixeles menos frecuentes a eliminar es mayor que el numero de pixeles unicos" in result_aos.stderr.decode())
        self.assertTrue("El numero de pixeles menos frecuentes a eliminar es mayor que el numero de pixeles unicos" in result_soa.stderr.decode())

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
        self.check_images("out_aos.ppm", "./expected/compress/deer-small.ppm")
        self.check_images("out_soa.ppm", "./expected/compress/deer-small.ppm")

    def test_compress_ok2(self):
        result_aos = subprocess.run(f'{exe_imtoolaos} ./input/lake-small.ppm out_aos.cppm compress', shell=True, capture_output=True)
        result_soa = subprocess.run(f'{exe_imtoolsoa} ./input/lake-small.ppm out_soa.cppm compress', shell=True, capture_output=True)
        self.assertEqual(result_aos.returncode, 0)
        self.assertEqual(result_soa.returncode, 0)

        # recuperamos las imágenes comprimidas y la original
        subprocess.run(f'./recover out_aos.cppm', shell=True)
        subprocess.run(f'./recover out_soa.cppm', shell=True)
        subprocess.run(f'./recover ./expected/compress/lake-small.cppm', shell=True)

        # comparamos las imágenes recuperadas con la original
        self.check_images("out_aos.ppm", "./expected/compress/lake-small.ppm")
        self.check_images("out_soa.ppm", "./expected/compress/lake-small.ppm")


if __name__ == "__main__":
    # Compilamos la herramienta recover
    subprocess.run("g++ recover.cpp -o recover", shell=True)
    unittest.main()