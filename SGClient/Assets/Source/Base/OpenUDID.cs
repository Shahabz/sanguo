using System;

namespace OpenUDIDPhone
{
    public static class OpenUDID
    {
        public enum OpenUDIDErrors
        {
            None = 0,
            OptedOut = 1,
            Compromised = 2
        }
        private static String _cachedValue;
        private static String _cachedDeviceUniqueId;
        private static OpenUDIDErrors _lastError;

        private static String _GetMacAddress()
        {
            if (_cachedDeviceUniqueId == null)
            {
                _cachedDeviceUniqueId = NetWorkInfo.GetMacAddress();
            }
            return _cachedDeviceUniqueId;
        }

        private static String _getOpenUDID()
        {
            _lastError = OpenUDIDErrors.None;
            if (_cachedValue == null)
            {
                _cachedValue = Utils.md5(_GetMacAddress());

            }
            return _cachedValue;
        }

        public static String MacAddress
        {
            get
            {
                return _GetMacAddress();
            }
        }
        public static String value
        {
            get
            {
                return _getOpenUDID();
            }
        }
        public static String valueWithError(out OpenUDIDErrors error)
        {
            String v = value;
            error = _lastError;
            return v;
        }
        public static String CorpIdentifier;
        public static String CorpValue
        {
            get
            {
                return  Utils.md5(String.Format("{0}.{1}", CorpIdentifier, value));
            }
        }
        public static String GetCorpUDID(String corpIdentifier)
        {
            CorpIdentifier = corpIdentifier;
            return CorpValue;
        }
    }
}
