import unittest
import random
import subprocess

version = "debug"
exe_imtoolaos = f"../cmake-build-{version}/imtool-aos/imtool-aos"

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

    def test_resize_ok1(self):
        result = subprocess.run(f'{exe_imtoolaos} ./input/deer-large.ppm out.ppm resize 100 100', shell=True, capture_output=True)
        self.assertEqual(result.returncode, 0)
        self.check_images("out.ppm", "./expected/resize/deer-large-100.ppm")

    def test_resize_ok2(self):
        result = subprocess.run(f'{exe_imtoolaos} ./input/deer-large.ppm out.ppm resize 1000 1000', shell=True, capture_output=True)
        self.assertEqual(result.returncode, 0)
        self.check_images("out.ppm", "./expected/resize/deer-large-1000.ppm")

    def test_resize_ok3(self):
        result = subprocess.run(f'{exe_imtoolaos} ./input/deer-small.ppm out.ppm resize 100 100', shell=True, capture_output=True)
        self.assertEqual(result.returncode, 0)
        self.check_images("out.ppm", "./expected/resize/deer-small-100.ppm")

    def test_resize_ok4(self):
        result = subprocess.run(f'{exe_imtoolaos} ./input/deer-small.ppm out.ppm resize 1000 1000', shell=True, capture_output=True)
        self.assertEqual(result.returncode, 0)
        self.check_images("out.ppm", "./expected/resize/deer-small-1000.ppm")




if __name__ == "__main__":
    unittest.main()