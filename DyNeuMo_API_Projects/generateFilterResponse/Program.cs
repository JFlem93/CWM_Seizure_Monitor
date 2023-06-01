// See https://aka.ms/new-console-template for more information

using PicostimApi;
using PicostimApi.Filter;
using System.Text.Json;


// Attempt to read from stdin
try
{
    // Read the filter type (0 = highpass, 1 = lowpass, 2 = bandpass, 4 = exponential moving average filter)
    int num_input_args = args.Length;
    int input_filter_type = short.Parse(args[0]); 
    string input_filter_response_filename = args[num_input_args-1];  // Example: "C:\\Users\\ndcm1133\\OneDrive - Nexus365\\Desktop\\highpass_test_filter_response.csv"

    if (input_filter_type == 0)   // Highpass
    {
        // Read the input parameter for the highpass
        string input_filter_coeffs = args[1];

        // Highpass takes a single byte as input
        Byte filter_coeff = Byte.Parse(input_filter_coeffs);

        // Initialize highpass filter coefficient parameter
        var highpassParameter = new OffsetParameter();
        highpassParameter.Shift = filter_coeff;

        // Convert the highpass parameter object to a filter
        var highpassFilter = highpassParameter.ToFilter();

        // Save the filter response to the specified file location
        highpassFilter.SaveResponse(input_filter_response_filename);

    }
    else if (input_filter_type == 1)   // lowpass
    {
        // Read the input parameter for the highpass
        string numBiquadsString = args[1];

        // Number of biquad stages
        short numBiquads = short.Parse(numBiquadsString);

        // Define a list for the number of biquad stages
        List<BiquadParameter> lowpassBiquadParameters = new List<BiquadParameter>();

        // Read the input parameters for the lowpass - could be multiple biquads, so need to loop through stdin
        for (int i = 1; i <= numBiquads; i++)
        {
            string biquad_coeffs = File.ReadAllText(@"./DyNeuMo_Lowpass_Filter_Biquads/" + i.ToString() + ".json");
            lowpassBiquadParameters.Add(JsonSerializer.Deserialize<BiquadParameter>(biquad_coeffs));

        }

        // Convert the list of biquad parameters to a lowpass filter
        var lowpassFilter = lowpassBiquadParameters.ToFilter();

        // Save the filter response to the specified file location
        lowpassFilter.SaveResponse(input_filter_response_filename);

    }
    else if (input_filter_type == 2)   // bandpass
    {
        // Read the input parameter for the highpass
        string numBiquadsString = args[1];

        // Number of biquad stages
        short numBiquads = short.Parse(numBiquadsString);

        // Define a list for the number of biquad stages
        List<BiquadParameter> bandpassBiquadParameters = new List<BiquadParameter>();

        // Read the input parameters for the bandpass - could be multiple biquads, so need to loop through stdin
        for (int i = 1; i <= numBiquads; i++)
        {
            string biquad_coeffs = File.ReadAllText(@"./DyNeuMo_Bandpass_Filter_Biquads/" + i.ToString() + ".json");
            bandpassBiquadParameters.Add(JsonSerializer.Deserialize<BiquadParameter>(biquad_coeffs));

        }

        // Convert the list of biquad parameters to a bandpass filter
        var bandpassFilter = bandpassBiquadParameters.ToFilter();

        // Save the filter response to the specified file location
        bandpassFilter.SaveResponse(input_filter_response_filename);

    }
    else if (input_filter_type == 4)   // lowpass - exponential moving average implementation
    {
        // Read the input parameter for the lowpass
        string input_filter_coeffs = args[1];

        // Lowpass takes a single byte as input
        Byte filter_coeff = Byte.Parse(input_filter_coeffs);

        // Initialize lowpass filter coefficient parameter
        var lowpassParameter = new ExponentialMovingAverageParameter();
        lowpassParameter.Shift = filter_coeff;

        // Convert the lowpass parameter object to a filter
        var lowpassFilter = lowpassParameter.ToFilter();

        // Save the filter response to the specified file location
        lowpassFilter.SaveResponse(input_filter_response_filename);

    }
    else 
    {
        Console.WriteLine("Undefined filter type specified");
    }

}
catch (Exception ex)
{
    Console.WriteLine(ex.Message);
}
