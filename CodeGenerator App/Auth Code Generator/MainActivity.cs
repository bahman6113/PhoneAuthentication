using System;
using Android.App;
using Android.Content;
using Android.Runtime;
using Android.Views;
using Android.Widget;
using Android.OS;
using System.Security.Cryptography;
using System.IO;

namespace Auth_Code_Generator
{
    [Activity(Label = "Auth_Code_Generator", MainLauncher = true, Icon = "@drawable/icon")]
    public class MainActivity : Activity
    {
        private readonly string mPass = "ThisIsJustASample,ThisIsNotASecureWayToStorePassword";
        private readonly byte[] mSalt = new byte[9] { 8, 7, 6, 5, 4, 3, 2, 1, 0 };
        private readonly byte[] mIV = new byte[16] { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
                                                      10, 11, 12, 13, 14, 15 };
        private readonly int mItrCount = 100;

        private byte[] CreateKey(string password, byte[] salt, int iteration)
        {
            Rfc2898DeriveBytes drvKey = new Rfc2898DeriveBytes(password, salt, iteration);
            return drvKey.GetBytes(32);
        }

        private string Encrypt(string plain)
        {
            using (Aes aes = Aes.Create())
            {
                aes.KeySize = 256;
                aes.BlockSize = 128;
                aes.Mode = CipherMode.CBC;
                aes.Padding = PaddingMode.Zeros;
                aes.Key = CreateKey(mPass, mSalt, mItrCount);
                aes.IV = mIV;
                ICryptoTransform encryptor = aes.CreateEncryptor(aes.Key, aes.IV);
                byte[] plainArray = System.Text.Encoding.ASCII.GetBytes(plain);
                byte[] encrypted = encryptor.TransformFinalBlock(plainArray, 0, plainArray.Length);
                byte[] hash = HashSHA256(encrypted);
                return Convert.ToBase64String(hash).Substring(0, 5);

            }
        }

        private byte[] HashSHA256(byte[] value)
        {
            using (var sha = SHA1.Create())
            {
                return sha.ComputeHash(value);
            }
        }

       
        private void GenerateCode()
        {
            EditText edtNonce = FindViewById<EditText>(Resource.Id.edt_Nonce);
            EditText edtCode = FindViewById<EditText>(Resource.Id.edt_Code);
            String code = Encrypt(edtNonce.Text);
            edtCode.Text = code;
        }
        protected override void OnCreate(Bundle bundle)
        {
            base.OnCreate(bundle);

            // Set our view from the "main" layout resource
            SetContentView(Resource.Layout.Main);

            // Get our button from the layout resource,
            // and attach an event to it
            Button button = FindViewById<Button>(Resource.Id.btn_Generate);

            button.Click += delegate { GenerateCode(); };
        }
    }
}

