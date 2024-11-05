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
        result = subprocess.run(f'{exe_imtoolaos} {self.img1_f3} out.ppm maxlevel 200', shell=True, capture_output=True)
        self.assertEqual(result.returncode, 0)
        self.assertTrue("Max level: 255" in result.stdout.decode())



if __name__ == "__main__":
    unittest.main()